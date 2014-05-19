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

#ifndef JSONPARSER_SELECTIONKEY_H
#define JSONPARSER_SELECTIONKEY_H

#include <jsonbus/core/shareddata.h>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/sharedptr.h>
#include "channel.h"
#include <sys/epoll.h>

/**
 * @namespace
 */
namespace JSONBus {

/**
 * @brief SelectionKey
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class SelectionKey {
public:
	
	/**
	 * @brief SelectionKey destructor
	 */
	~SelectionKey();
	
	void cancel();
	
	bool isReadable();
	
	bool isWritable();
	
	bool isValid();

private:
	/**
	 * @brief SelectionKey constructor
	 */
	SelectionKey();
	
	bool m_valid;
};

typedef SharedPtr<SelectionKey> SelectionKeyPtr;


}

#endif // JSONPARSER_SELECTIONKEY_H
