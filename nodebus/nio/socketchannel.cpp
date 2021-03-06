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

#include "socketchannel.h"
#include <nodebus/core/logger.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <QString>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(strerror(errno)))

#define THROW_IOEXP(msg) \
	throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + (msg == NULL ? strerror(errno) : msg))

namespace NodeBus {

static int __connect(const QString &host, int port) {
	addrinfo *addrinfo = NULL, *it;
	bool connected = false;
	const char* error = NULL;
	int fd = -1;
	QRegExp regex("^\\s*\\[([a-fA-F0-9:]+)\\]\\s*$");
	do {
		if (::getaddrinfo((regex.indexIn(host) > -1 ? regex.cap(1).toStdString().c_str() : 
			host.toStdString().c_str()), NULL, NULL, &addrinfo) == -1) {
			error = ::hstrerror(h_errno);
			break;
		}
		it = addrinfo;
		do {
			fd = ::socket(it->ai_family, SOCK_STREAM, 0);
			if (fd == -1)
				break;
			((struct sockaddr_in *)(it->ai_addr))->sin_port = htons(port);
			connected |= (::connect(fd, it->ai_addr, it->ai_addrlen) == 0);
			if (connected)
				break;
			::close(fd);
			it = it->ai_next;
		} while(it);
	} while (false);
	if (addrinfo) {
		::freeaddrinfo(addrinfo);
	}
	if (connected)
		return fd;
	if (error) {
		THROW_IOEXP(error);
	} else {
		THROW_IOEXP(nullptr);
	}
}

SocketChannel::SocketChannel(const QString& host, int port): IOChannel(__connect(host, port), true), m_name(host + ":" + QString::number(port)) {
	logFiner() << "SocketChannel::connected to " << m_name;
}

SocketChannel::SocketChannel(int fd, const QString &name): IOChannel(fd, true), m_name(name) {
	logFiner() << "SocketChannel::connected from " << m_name;
}

SocketChannel::~SocketChannel() {
	logFiner() << "SocketChannel::disconnected from " << m_name;
}

}
