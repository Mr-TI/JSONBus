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

#ifndef JSONPARSER_ABSTRACTSTREAMBUF_H
#define JSONPARSER_ABSTRACTSTREAMBUF_H

#include <iostream>
#include <fstream>
#include <jsonbus/core/shareddata.h>
#include "sharedptr.h"

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
class AbstractChannel: public SharedData {
public:
	/**
	 * @brief AbstractChannel constructor
	 */
	AbstractChannel();
	
	/**
	 * @brief AbstractChannel destructor
	 */
	virtual ~AbstractChannel() = 0;
	
	/**
	 * @brief Disable the buffer
	 */
	virtual void close() = 0;
	
	/**
	 * @brief underflow
	 * @return a charracter or EOF
	 */
	virtual int read() = 0;
	
	/**
	 * @brief underflow
	 * @return a charracter or EOF
	 */
	virtual int read(char buffer, int maxlen) = 0;
	
	/**
	 * @brief underflow
	 * @return a charracter or EOF
	 */
	virtual void write(int val) = 0;
	
	/**
	 * @brief underflow
	 * @return a charracter or EOF
	 */
	virtual void write(char buffer, int len) = 0;
};

typedef SharedPtr<AbstractChannel> ChannelPtr;

}

#endif // JSONPARSER_ABSTRACTSTREAMBUF_H
