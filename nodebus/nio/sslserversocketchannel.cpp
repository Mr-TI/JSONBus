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

#include "sslserversocketchannel.h"
#include <nodebus/core/logger.h>
#include "iochannel.h"
#include "ssliochannel.h"
#include "sslsocketchannel.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <QString>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(strerror(errno)))

#define THROW_IOEXP(msg) \
	throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(msg == NULL ? strerror(errno) : msg))

namespace NodeBus {

SSLServerSocketChannel::SSLServerSocketChannel(const QString& host, int port, SSLContextPtr ctx, uint opts): ServerSocketChannel(host, port, opts), m_ctx(ctx) {
}

SSLServerSocketChannel::~SSLServerSocketChannel() {

}

void SSLServerSocketChannel::close() {
	ServerSocketChannel::close();
}

#define CLIENT_HOST_MAXLEN 256
#define CLIENT_SERV_MAXLEN 32

SocketChannelPtr SSLServerSocketChannel::accept() {
	int cldf;
	QString name;
	s_accept(cldf, name);
	return new SSLSocketChannel(cldf, name, m_ctx);
}

}
