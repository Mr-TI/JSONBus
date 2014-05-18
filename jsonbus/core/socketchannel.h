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

#ifndef JSONPARSER_SOCKETCHANNEL_H
#define JSONPARSER_SOCKETCHANNEL_H

#include <jsonbus/core/exception.h>
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
class SocketChannel: public IOChannel {
public:
	/**
	 * @brief Socket constructor
	 */
	SocketChannel(const QString &host, int port);
	
	/**
	 * @brief Socket destructor
	 */
	virtual ~SocketChannel();
};

typedef SharedPtr<SocketChannel> SocketPtr;

}

#endif // JSONPARSER_SOCKETCHANNEL_H
