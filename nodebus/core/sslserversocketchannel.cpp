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

#include "sslserversocketchannel.h"
#include "logger.h"
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
