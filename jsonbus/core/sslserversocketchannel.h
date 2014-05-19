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

#ifndef JSONPARSER_SSLSERVERSOCKETCHANNEL_H
#define JSONPARSER_SSLSERVERSOCKETCHANNEL_H

#include <jsonbus/core/exception.h>
#include <jsonbus/core/serversocketchannel.h>
#include <jsonbus/core/streamchannel.h>
#include <openssl/ssl.h>

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
class SSLServerSocketChannel: public ServerSocketChannel {
public:
	/**
	 * @brief Socket constructor
	 */
	SSLServerSocketChannel(const QString &host, int port, SSL_CTX *ctx);
	
	/**
	 * @brief Socket destructor
	 */
	virtual ~SSLServerSocketChannel();
	
	/**
	 * @brief Close the channel
	 * @throw IOException on error
	 */
	virtual void close();
	
	/**
	 * @brief Connect
	 */
	StreamChannelPtr accept();
	
	/**
	 * @brief Get the inner file descriptor if supported
	 * @return the inner file descriptor or -1 if not supported
	 */
	virtual int getFd();
	
protected:
	int m_fd;
	
private:
    SSL_CTX* m_ctx;
};

inline int SSLServerSocketChannel::getFd() {
	return m_fd;
}

}

#endif // JSONPARSER_SSLSERVERSOCKETCHANNEL_H
