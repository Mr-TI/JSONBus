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

#ifndef JSONPARSER_SOCKETCHANNEL_H
#define JSONPARSER_SOCKETCHANNEL_H

#include <nodebus/core/exception.h>
#include <nodebus/core/iochannel.h>

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
