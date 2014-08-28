/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
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

#include "ssliochannel.h"
#include "logger.h"
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) < 1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + getLastError())

#define THROW_IOEXP_ON_NULL(exp) \
	if ((exp) == nullptr) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + getLastError())

namespace NodeBus {

QString SSLIOChannel::getLastError() {
	QString msg;
	const char *txt;
	unsigned long int err = ERR_get_error();
	txt = ERR_func_error_string(err);
	if (txt != nullptr) {
		msg.append(QString::fromLocal8Bit(txt));
		msg.append(": ");
	}
	txt = ERR_reason_error_string(err);
	if (txt != nullptr) {
		msg.append(QString::fromLocal8Bit(txt));
	} else {
		msg.append(QString::fromLocal8Bit(strerror(errno)));
	}
	return msg;
}

SSLIOChannel::SSLIOChannel(int fd, SSL_CTX *ctx, bool closeOnDelete) : IOChannel(fd, closeOnDelete), m_ssl(nullptr) {
	THROW_IOEXP_ON_NULL(m_ssl = SSL_new(ctx));
	THROW_IOEXP_ON_ERR(SSL_set_fd(m_ssl, fd));
}

SSLIOChannel::~SSLIOChannel() {
	if (m_closeOnDelete && isOpen()) {
		close();
	}
	::SSL_free(m_ssl);
}

size_t SSLIOChannel::s_read(char *buffer, size_t maxlen) {
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

void SSLIOChannel::s_write(const char *buffer, size_t len) {
	ssize_t ret = 0;
	while (len > 0) {
		THROW_IOEXP_ON_ERR(ret = SSL_write(m_ssl, buffer, len));
		len -= ret;
		buffer += ret;
	}
}

void SSLIOChannel::close() {
	if (isOpen()) {
		::SSL_shutdown(m_ssl);
		IOChannel::close();
	}
}

size_t SSLIOChannel::s_available() {
	size_t result = SSL_pending(m_ssl);
	if (result > 0) {
		return result;
	}
	result = IOChannel::s_available();
	return result;
}

void SSLIOChannel::accept() {
	THROW_IOEXP_ON_ERR(::SSL_accept(m_ssl));
}

void SSLIOChannel::connect() {
	THROW_IOEXP_ON_ERR(::SSL_connect(m_ssl));
}

}
