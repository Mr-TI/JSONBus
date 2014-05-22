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

#ifndef JSONPARSER_SSLCHANNEL_H
#define JSONPARSER_SSLCHANNEL_H

#include <sys/epoll.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <jsonbus/core/shareddata.h>
#include <jsonbus/core/iochannel.h>

/**
 * @namespace
 */
namespace JSONBus {

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class SSLIOChannel: public IOChannel {
public:
	/**
	 * @brief AbstractChannel constructor
	 * @param fd a valid file descriptor
	 * @param ctx a valid ssl context
	 * @throw IOException on error
	 */
	SSLIOChannel(int fd, SSL_CTX *ctx, bool closeOnDelete = false);
	
	/**
	 * @brief AbstractChannel destructor
	 */
	virtual ~SSLIOChannel();
	
	/**
	 * @brief SSL session connect
	 * @throw IOException on error
	 */
	void connect();
	
	/**
	 * @brief SSL session accept
	 * @throw IOException on error
	 */
	void accept();
	
	/**
	 * @brief Close the channel
	 * @throw IOException on error
	 */
	virtual void close();
	
	static QString getLastError();
	
protected:
	virtual size_t s_available();
	virtual size_t s_read(char *buffer, size_t maxlen);
	virtual void s_write(const char *buffer, size_t len);

private:
	SSL *m_ssl;
};

}

#endif // JSONPARSER_SSLCHANNEL_H
