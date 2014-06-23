/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@openihs.org>
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *		   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include "iochannel.h"
#include "logger.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(strerror(errno)))

namespace NodeBus {

inline void StreamChannel::checkBuf() {
	if (m_readEnd == m_readStart) {
		size_t n;
		n = available(true);
		m_readEnd = s_read(m_readBuff, qMin(n, (size_t)sizeof(m_readBuff)));
		m_readStart = 0;
	}
}

char StreamChannel::get() {
	checkBuf();
	return m_readBuff[m_readStart++];
}

char StreamChannel::peek() {
	checkBuf();
	return m_readBuff[m_readStart];
}

void StreamChannel::ignore(size_t len) {
	for (size_t i=0; i < len; i++) {
		get();
	}
}

size_t StreamChannel::read(char *buffer, size_t maxlen) {
	size_t count;
	if (m_readEnd != m_readStart) {
		count = qMin(m_readEnd - m_readStart, maxlen);
		memcpy(buffer, m_readBuff + m_readStart, count);
		m_readStart += count;
		return count;
	}
	count = qMin(available(true), maxlen);
	return s_read(buffer, count);
}

void StreamChannel::write(const char *buffer, size_t len) {
	s_write(buffer, len);
}

size_t StreamChannel::available(bool noEmpty) {
	int n = s_available() + (m_readEnd - m_readStart);
	if (noEmpty && n == 0) {
		if (m_deadline == -1) {
			while (!s_waitForReadyRead(100)) {
				if (!m_open) {
					throw EOFException("Closed channel");
				}
			}
		} else {
			while (!s_waitForReadyRead(100)) {
				if (!m_open) {
					throw EOFException("Closed channel");
				}
				if (m_deadline < QDateTime::currentMSecsSinceEpoch()) {
					throw IOTimeoutException("Time exceeds");
				}
			}
		}
		n = s_available();
		if (n == 0) {
			throw EOFException("End of File");
		}
	}
	return n;
}

void StreamChannel::close() {
	Channel::close();
}


}
