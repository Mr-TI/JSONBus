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

#ifndef JSONPARSER_SSLSOCKETCHANNEL_H
#define JSONPARSER_SSLSOCKETCHANNEL_H

#include <sys/epoll.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <jsonbus/core/shareddata.h>
#include <jsonbus/core/socketchannel.h>
#include <jsonbus/core/sslcontext.h>

/**
 * @namespace
 */
namespace JSONBus {

class SSLServerSocketChannel;

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class SSLSocketChannel: public SocketChannel {
	friend SSLServerSocketChannel;
public:
	/**
	 * @brief AbstractChannel constructor
	 * @param fd a valid file descriptor
	 * @param ctx a valid ssl context
	 * @throw IOException on error
	 */
	SSLSocketChannel(const QString &host, int port, SSLContextPtr ctx);
	
	/**
	 * @brief AbstractChannel destructor
	 */
	virtual ~SSLSocketChannel();
	
	/**
	 * @brief Close the channel
	 * @throw IOException on error
	 */
	virtual void close();
	
protected:
	SSLSocketChannel(int fd, const QString &name, SSLContextPtr ctx);
	virtual size_t s_available();
	virtual size_t s_read(char *buffer, size_t maxlen);
	virtual void s_write(const char *buffer, size_t len);

private:
	SSL *m_ssl;
};

}

#endif // JSONPARSER_SSLSOCKETCHANNEL_H
