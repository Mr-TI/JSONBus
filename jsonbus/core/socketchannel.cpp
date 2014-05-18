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

#include "socketchannel.h"
#include "logger.h"
#include <sys/ioctl.h>
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

static int __connect(const QString &host, int port) {
	addrinfo *addrinfo, *it;
	bool connected;
	int fd;
	THROW_IOEXP_ON_ERR(fd = socket(addrinfo->ai_family, SOCK_STREAM, 0));
	if (getaddrinfo(host.toStdString().c_str(), NULL, NULL, &addrinfo) == -1) {
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

SocketChannel::SocketChannel(const QString &host, int port): IOChannel(__connect(host, port)) {
	
}

SocketChannel::~SocketChannel() {

}

}
