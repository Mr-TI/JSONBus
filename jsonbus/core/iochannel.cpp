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

size_t IOChannel::s_read(char *buffer, size_t maxlen) throw(IOException) {
	ssize_t ret;
	ret = ::read(m_fd, buffer, maxlen);
	THROW_IOEXP_ON_ERR(ret);
	return ret;
}

void IOChannel::s_write(const char *buffer, size_t len) throw(IOException) {
	int ret = 0;
	while (len > 0) {
		ret = ::write(m_fd, buffer, len);
		THROW_IOEXP_ON_ERR(ret);
		len -= ret;
		buffer += ret;
	}
}

void IOChannel::close() throw(IOException) {
	AbstractChannel::close();
	THROW_IOEXP_ON_ERR(::close(m_fd));
}

size_t IOChannel::s_available() throw(IOException) {
	size_t result;
	THROW_IOEXP_ON_ERR(::ioctl(m_fd, FIONREAD, &result));
	return result;
}


}
