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

#include "iochannel.h"
#include "logger.h"
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(strerror(errno)))

namespace NodeBus {

IOChannel::IOChannel(int fd, int flags) : m_fd(fd), m_closeOnDelete(flags & CLOSE_ON_DELETE)
#ifdef WIN32
	
#else //WIN32
, m_epfd(-1)
#endif //WIN32
{
#ifdef WIN32
	
#else //WIN32
	THROW_IOEXP_ON_ERR(m_epfd = epoll_create1(0));
	bzero(&m_event, sizeof(epoll_event));
	m_event.data.fd = m_fd;
	m_event.events = EPOLLIN | EPOLLET;
	if (flags & READABLE) {
		THROW_IOEXP_ON_ERR(epoll_ctl (m_epfd, EPOLL_CTL_ADD, m_fd, &m_event));
	}
#endif //WIN32
}

IOChannel::~IOChannel() {
	if (m_closeOnDelete && isOpen()) {
		close();
	}
#ifdef WIN32
	
#else //WIN32
	::close(m_epfd);
#endif //WIN32
}

size_t IOChannel::s_read(char *buffer, size_t maxlen) {
	ssize_t ret;
	ret = ::read(m_fd, buffer, maxlen);
	THROW_IOEXP_ON_ERR(ret);
	return ret;
}

void IOChannel::s_write(const char *buffer, size_t len) {
	int ret = 0;
	while (len > 0) {
		THROW_IOEXP_ON_ERR(ret = ::write(m_fd, buffer, len));
		len -= ret;
		buffer += ret;
	}
}

void IOChannel::closeFd() {
	::close(m_fd);
}

size_t IOChannel::s_available() {
	size_t result;
	s_waitForReadyRead(0);
	THROW_IOEXP_ON_ERR(::ioctl(m_fd, FIONREAD, &result));
	return result;
}

bool IOChannel::s_waitForReadyRead(int timeout) {
#ifdef WIN32
	throw UnsupportedOperationException();
#else //WIN32
	int ret;
	THROW_IOEXP_ON_ERR(ret = epoll_wait(m_epfd, m_events,1 ,timeout));
	if (ret != 1) {
		return false;
	}
	if (m_events[0].events & EPOLLHUP) {
		throw EOFException();
	}
	if (m_events[0].events & EPOLLERR) {
		throw IOException(QString() + __FILE__ + ":" + __LINE__ + ": " + strerror(errno));
	}
	return m_events[0].events & EPOLLIN;
#endif //WIN32
}

void IOChannel::updateStatus(int events) {
#ifdef WIN32
	
#else //WIN32
	if (events & EPOLLIN) {
		int result;
		if (::ioctl(m_fd, FIONREAD, &result) == -1 || result == 0) {
			close();
		}
	}
#endif //WIN32
}

}
