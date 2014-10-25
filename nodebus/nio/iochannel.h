/*
 * Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef JSONPARSER_IOCHANNEL_H
#define JSONPARSER_IOCHANNEL_H

#ifdef WIN32
	
#else //WIN32
#	include <sys/epoll.h>
#endif //WIN32
#include <nodebus/core/shareddata.h>
#include <nodebus/nio/streamchannel.h>

/**
 * @namespace
 */
namespace NodeBus {

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class IOChannel: public StreamChannel {
public:
	enum Flags {
		CLOSE_ON_DELETE = 0x01,
		READABLE = 0x02,
		WRITABLE = 0x04
	};
	/**
	 * @brief AbstractChannel constructor
	 * @param fd a valid file descriptor
	 * @throw IOException on error
	 */
	IOChannel(int fd, int flags = READABLE | WRITABLE);
	
	/**
	 * @brief AbstractChannel destructor
	 */
	virtual ~IOChannel();
	
protected:
	virtual int &fd();
	virtual void closeFd();
	virtual void updateStatus(int events);
	virtual size_t s_available();
	virtual size_t s_read(char *buffer, size_t maxlen);
	virtual void s_write(const char *buffer, size_t len);
	bool s_waitForReadyRead(int timeout);
	int m_fd;
	bool m_closeOnDelete;

private:
#ifdef WIN32
	
#else //WIN32
	int m_epfd;
	epoll_event m_event;
	epoll_event m_events[1];
#endif //WIN32
};

inline int &IOChannel::fd() {
	return m_fd;
}

}

#endif // JSONPARSER_IOCHANNEL_H
