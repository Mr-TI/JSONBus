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

#ifndef JSONPARSER_SOCKETCHANNEL_H
#define JSONPARSER_SOCKETCHANNEL_H

#include <nodebus/core/exception.h>
#include <nodebus/nio/iochannel.h>

/**
 * @namespace
 */
namespace NodeBus {

class ServerSocketChannel;

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class SocketChannel: public IOChannel {
	friend class ServerSocketChannel;
public:
	/**
	 * @brief Socket constructor
	 */
	SocketChannel(const QString &host, int port);
	
	/**
	 * @brief Socket destructor
	 */
	virtual ~SocketChannel();
	
protected:
	SocketChannel(int fd, const QString &name);
	
private:
	QString m_name;
};

typedef SharedPtr<SocketChannel> SocketChannelPtr;

}

#endif // JSONPARSER_SOCKETCHANNEL_H
