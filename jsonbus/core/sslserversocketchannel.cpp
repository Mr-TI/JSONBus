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
#include "sslchannel.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <QString>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + __LINE__ + ": " + strerror(errno))

#define THROW_IOEXP(msg) \
	throw IOException(QString() + __FILE__ + ":" + __LINE__ + ": " + (msg == NULL ? strerror(errno) : msg))

namespace JSONBus {

SSLServerSocketChannel::SSLServerSocketChannel(const QString &host, int port, SSL_CTX *ctx): ServerSocketChannel(host, port), m_ctx(ctx) {
}

SSLServerSocketChannel::~SSLServerSocketChannel() {

}

void SSLServerSocketChannel::close() {
	THROW_IOEXP_ON_ERR(::close(m_fd));
}

#define CLIENT_HOST_MAXLEN 256
#define CLIENT_SERV_MAXLEN 32

ChannelPtr SSLServerSocketChannel::accept() {
	char client_host[CLIENT_HOST_MAXLEN + 1];
	client_host[0] = '\0';
	char client_serv[CLIENT_SERV_MAXLEN + 1];
	client_serv[0] = '\0';
	struct sockaddr sockaddr_client;
	socklen_t sockaddr_len = sizeof(struct sockaddr);
	int cldf = ::accept(m_fd, &sockaddr_client, &sockaddr_len);
	THROW_IOEXP_ON_ERR(cldf);
	if (getnameinfo(&sockaddr_client, sockaddr_len, client_host, CLIENT_HOST_MAXLEN, client_serv, CLIENT_SERV_MAXLEN, 0) == -1) {
		THROW_IOEXP(hstrerror(h_errno));
	}
	logFiner() << "SSLServerSocketChannel: New connection from " << client_host << ":" << client_serv;
	SharedPtr<SSLChannel> channel = new SSLChannel(cldf, m_ctx);
	channel->accept();
	return channel;
}

}
