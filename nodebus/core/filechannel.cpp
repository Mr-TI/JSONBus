/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
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

#include "filechannel.h"
#include "logger.h"
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(strerror(errno)))

namespace NodeBus {

FileChannel::FileChannel(const QString &path, int flags, int mode) : m_fd(-1) {
	THROW_IOEXP_ON_ERR(m_fd = open(path.toLocal8Bit().constData(), flags, mode));
}

FileChannel::~FileChannel() {
	if (isOpen()) {
		close();
	}
}

size_t FileChannel::s_read(char *buffer, size_t maxlen) {
	ssize_t ret;
	ret = ::read(m_fd, buffer, maxlen);
	THROW_IOEXP_ON_ERR(ret);
	return ret;
}

void FileChannel::s_write(const char *buffer, size_t len) {
	int ret = 0;
	while (len > 0) {
		THROW_IOEXP_ON_ERR(ret = ::write(m_fd, buffer, len));
		len -= ret;
		buffer += ret;
	}
}

void FileChannel::closeFd() {
	::close(m_fd);
}

size_t FileChannel::s_available() {
	size_t result = 0;
	s_waitForReadyRead(0);
	THROW_IOEXP_ON_ERR(::ioctl(m_fd, FIONREAD, &result));
	return result;
}

bool FileChannel::s_waitForReadyRead(int timeout) {
	return true;
}

}
