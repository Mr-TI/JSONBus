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

#include "sslsocketchannel.h"
#include <nodebus/core/logger.h>
#include "ssliochannel.h"
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <openssl/err.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) < 1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + SSLIOChannel::getLastError())

#define THROW_IOEXP_ON_NULL(exp) \
	if ((exp) == nullptr) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + SSLIOChannel::getLastError())

namespace NodeBus {

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
	ssize_t n = SSL_read(m_ssl, buffer, maxlen);
	if (n == 0) {
		int e = SSL_get_error(m_ssl, n);
		if (e == SSL_ERROR_NONE || e == SSL_ERROR_ZERO_RETURN) {
			throw EOFException();
		}
	}
	THROW_IOEXP_ON_ERR(n);
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
