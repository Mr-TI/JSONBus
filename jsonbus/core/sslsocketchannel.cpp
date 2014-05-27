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

#include "sslsocketchannel.h"
#include "logger.h"
#include "ssliochannel.h"
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <openssl/err.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) < 1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + SSLIOChannel::getLastError())

#define THROW_IOEXP_ON_NULL(exp) \
	if ((exp) == nullptr) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + SSLIOChannel::getLastError())

namespace JSONBus {

SSLSocketChannel::SSLSocketChannel(const QString& host, int port, SSLContextPtr ctx) : SocketChannel(host, port), m_ssl(NULL) {
	THROW_IOEXP_ON_NULL(m_ssl = SSL_new(ctx->getCTX()));
	try {
		THROW_IOEXP_ON_ERR(SSL_set_fd(m_ssl, m_fd));
		THROW_IOEXP_ON_ERR(::SSL_connect(m_ssl));
	} catch (Exception &e) {
		SSL_free(m_ssl);
		throw e;
	}
}

SSLSocketChannel::SSLSocketChannel(int fd, const QString& name, SSLContextPtr ctx) : SocketChannel(fd, name), m_ssl(NULL) {
	THROW_IOEXP_ON_NULL(m_ssl = SSL_new(ctx->getCTX()));
	try {
		THROW_IOEXP_ON_ERR(SSL_set_fd(m_ssl, m_fd));
		THROW_IOEXP_ON_ERR(::SSL_accept(m_ssl));
	} catch (Exception &e) {
		SSL_free(m_ssl);
		throw e;
	}
}


SSLSocketChannel::~SSLSocketChannel() {
	if (isOpen()) {
		close();
	}
	::SSL_free(m_ssl);
}

size_t SSLSocketChannel::s_read(char *buffer, size_t maxlen) {
	ssize_t n;
	THROW_IOEXP_ON_ERR(n = SSL_read(m_ssl, buffer, maxlen));
	return n;
}

void SSLSocketChannel::s_write(const char *buffer, size_t len) {
	ssize_t ret = 0;
	while (len > 0) {
		THROW_IOEXP_ON_ERR(ret = SSL_write(m_ssl, buffer, len));
		len -= ret;
		buffer += ret;
	}
}

void SSLSocketChannel::close() {
	if (isOpen()) {
		::SSL_shutdown(m_ssl);
		IOChannel::close();
	}
}

size_t SSLSocketChannel::s_available() {
	size_t result = SSL_pending(m_ssl);
	if (result > 0) {
		return result;
	}
	result = IOChannel::s_available();
	return result;
}

}
