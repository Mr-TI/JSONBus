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

#ifndef JSONPARSER_SSLSOCKETCHANNEL_H
#define JSONPARSER_SSLSOCKETCHANNEL_H

#include <openssl/ssl.h>
#include <nodebus/core/shareddata.h>
#include <nodebus/nio/socketchannel.h>
#include <nodebus/nio/sslcontext.h>

/**
 * @namespace
 */
namespace NodeBus {

class SSLServerSocketChannel;

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class SSLSocketChannel: public SocketChannel {
	friend class SSLServerSocketChannel;
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
