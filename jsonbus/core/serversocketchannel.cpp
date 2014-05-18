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

#include "serversocketchannel.h"
#include "logger.h"
#include "iochannel.h"
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

inline int __bind(const QString &host, int port) {
	addrinfo *addrinfo, *it;
	bool connected;
	int fd;
	THROW_IOEXP_ON_ERR(fd = socket(addrinfo->ai_family, SOCK_STREAM, 0));
	QRegExp regex("^\\s*\\[([a-fA-F0-9:]+)\\]\\s*$");
	if (getaddrinfo((regex.indexIn(host) > -1 ? regex.cap(1).toStdString().c_str() : 
		host.toStdString().c_str()), NULL, NULL, &addrinfo) == -1) {
		THROW_IOEXP(hstrerror(h_errno));
	}
	((struct sockaddr_in *)(addrinfo->ai_addr))->sin_port = htons(port);
	it = addrinfo;
	do {
		connected |= (connect(fd, it->ai_addr, it->ai_addrlen) == 0);
		it = it->ai_next;
	} while(!connected && it);
	freeaddrinfo(addrinfo);
	if (!connected) {
		THROW_IOEXP(nullptr);
	}
	return fd;
}

ServerSocketChannel::ServerSocketChannel(const QString &host, int port) {
	m_fd = __bind(host, port);
}

ServerSocketChannel::~ServerSocketChannel() {

}

void ServerSocketChannel::close() {
	THROW_IOEXP_ON_ERR(::close(m_fd));
}

#define CLIENT_HOST_MAXLEN 256
#define CLIENT_SERV_MAXLEN 32

ChannelPtr ServerSocketChannel::accept() {
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
	logFiner() << "ServerSocketChannel: New connection from " << client_host << ":" << client_serv;
	return new IOChannel(cldf);
}

}
