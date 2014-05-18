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

#include "sslchannel.h"
#include "logger.h"
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) < 1) throw IOException(QString() + __FILE__ + ":" + __LINE__ + ": " + getLastError())

#define THROW_IOEXP_ON_NULL(exp) \
	if ((exp) == nullptr) throw IOException(QString() + __FILE__ + ":" + __LINE__ + ": " + getLastError())

namespace JSONBus {

QString SSLChannel::getLastError() {
	QString msg;
	const char *txt;
	unsigned long int err = ERR_get_error();
	txt = ERR_func_error_string(err);
	if (txt != nullptr) {
		msg.append(txt);
		msg.append(": ");
	}
	txt = ERR_reason_error_string(err);
	if (txt != nullptr) {
		msg.append(txt);
	} else {
		msg.append(strerror(errno));
	}
	return msg;
}

SSLChannel::SSLChannel(int fd, SSL_CTX *ctx) : IOChannel(fd), m_ssl(nullptr) {
	THROW_IOEXP_ON_NULL(m_ssl = SSL_new(ctx));
}

SSLChannel::~SSLChannel() {
}

size_t SSLChannel::s_read(char *buffer, size_t maxlen) {
	ssize_t n;
	THROW_IOEXP_ON_ERR(n = SSL_read(m_ssl, buffer, maxlen));
	return n;
}

void SSLChannel::s_write(const char *buffer, size_t len) {
	ssize_t ret = 0;
	while (len > 0) {
		THROW_IOEXP_ON_ERR(ret = SSL_write(m_ssl, buffer, len));
		len -= ret;
		buffer += ret;
	}
}

void SSLChannel::close() {
	THROW_IOEXP_ON_ERR(::SSL_shutdown(m_ssl));
	IOChannel::close();
}

size_t SSLChannel::s_available() {
	size_t result = SSL_pending(m_ssl);
	if (result > 0) {
		return result;
	}
	result = IOChannel::s_available();
	return result;
}

void SSLChannel::accept() {
	THROW_IOEXP_ON_ERR(::SSL_accept(m_ssl));
}

void SSLChannel::connect() {
	THROW_IOEXP_ON_ERR(::SSL_connect(m_ssl));
}

}
