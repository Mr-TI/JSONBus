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
#include <string.h>
#include <unistd.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + __LINE__ + ": " + strerror(errno))

namespace JSONBus {

inline void AbstractChannel::checkBuf() {
	if (m_readEnd == m_readStart) {
		size_t n;
		n = available(true);
		m_readEnd = s_read(m_readBuff, qMin(n, (size_t)sizeof(m_readBuff)));
		m_readStart = 0;
	}
}

char AbstractChannel::get() throw(IOException) {
	checkBuf();
	return m_readBuff[m_readStart++];
}

char AbstractChannel::peek() throw(IOException) {
	checkBuf();
	return m_readBuff[m_readStart];
}

void AbstractChannel::ignore(size_t len) throw(IOException) {
	for (size_t i=0; i < len; i++) {
		get();
	}
}

size_t AbstractChannel::read(char *buffer, size_t maxlen) throw(IOException) {
	size_t count;
	if (m_readEnd != m_readStart) {
		count = qMin(m_readEnd - m_readStart, maxlen);
		memccpy(buffer, m_readBuff, count, 1);
		m_readStart += count;
		return count;
	}
	count = qMin(available(true), maxlen);
	return s_read(buffer, count);
}

void AbstractChannel::write(const char *buffer, size_t len) throw(IOException) {
	s_write(buffer, len);
}

size_t AbstractChannel::available(bool noEmpty) throw (IOException) {
	int n = s_available();
	if (noEmpty && n == 0) {
		int fd = getFd();
		if (fd == -1) {
			throw EOFException("End of File");
		}
		if (m_deadline.tv_sec == 0) {
			timeval waitime;
			do {
				if (!m_enabled) {
					throw EOFException("Closed channel");
				}
				waitime.tv_sec = 0;
				waitime.tv_usec = 100000;
				FD_ZERO(&(m_readfds));
				FD_SET(fd, &(m_readfds));
				THROW_IOEXP_ON_ERR(n = select(fd + 1, &(m_readfds), nullptr, nullptr, &waitime));
			} while (n == 0);
		} else {
			timeval curtime, waitime;
			do {
				if (!m_enabled) {
					throw EOFException("Closed channel");
				}
				THROW_IOEXP_ON_ERR(gettimeofday(&curtime, nullptr));
				if (m_deadline.tv_sec < curtime.tv_sec || (m_deadline.tv_sec == curtime.tv_sec && m_deadline.tv_usec < curtime.tv_usec)) {
					throw IOException("Time exceeds");
				}
				waitime.tv_sec = 0;
				waitime.tv_usec = 100000;
				FD_ZERO(&(m_readfds));
				FD_SET(fd, &(m_readfds));
				THROW_IOEXP_ON_ERR(n = select(fd + 1, &(m_readfds), nullptr, nullptr, &waitime));
			} while (n == 0);
		}
		n = s_available();
		if (n == 0) {
			throw EOFException("End of File");
		}
	}
	return n;
}


}
