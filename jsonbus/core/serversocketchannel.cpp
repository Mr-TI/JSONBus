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

static int __bind(const QString &host, int port, int listenQueueSize) {
	addrinfo *addrinfo = NULL, *it;
	bool bound = false;
	const char* error = NULL;
	int fd = -1, ret;
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
			ret = ::bind(fd, it->ai_addr, it->ai_addrlen);
			bound |= (ret == 0);
			if (bound)
				break;
			::close(fd);
			it = it->ai_next;
		} while(it);
	} while (false);
	if (addrinfo) {
		freeaddrinfo(addrinfo);
	}
	if (bound) {
		THROW_IOEXP_ON_ERR(listen(fd, 10));
		return fd;
	}
	if (error) {
		THROW_IOEXP(error);
	} else {
		THROW_IOEXP(nullptr);
	}
}

ServerSocketChannel::ServerSocketChannel(const QString &host, int port, int listenQueueSize): m_fd(__bind(host, port, listenQueueSize)) {
}

ServerSocketChannel::~ServerSocketChannel() {

}

void ServerSocketChannel::close() {
	logFinest() << "ServerSocketChannel::close()";
	::close(m_fd);
}

#define CLIENT_HOST_MAXLEN 256
#define CLIENT_SERV_MAXLEN 32

StreamChannelPtr ServerSocketChannel::accept() {
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
