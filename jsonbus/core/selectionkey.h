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
#include <sys/epoll.h>

/**
 * @namespace
 */
namespace JSONBus {

class Channel;
class Selector;

/**
 * @brief SelectionKey
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class SelectionKey : public SharedData {
	friend class Channel;
	friend class Selector;
public:
	enum Flags {
		OP_READ = EPOLLIN,
		OP_WRITE = EPOLLOUT
	};
	
	/**
	 * @brief SelectionKey destructor
	 */
	~SelectionKey();
	
	/**
	 * @brief Cancel it
	 */
	void cancel();
	
	/**
	 * @brief Get the associated channel
	 * @return a pointer to the channel or null if this channel is not active
	 */
	SharedPtr<Channel> channel();
	
	/**
	 * @brief Return if the channel is readable
	 * @return true if it is readable, otherwise false
	 */
	bool isReadable();
	
	/**
	 * @brief Return if the channel is writable
	 * @return true if it is writable, otherwise false
	 */
	bool isWritable();
	
	/**
	 * @brief Return if the channel is valid
	 * @return true if it is valid, otherwise false
	 */
	bool isValid();

private:
	/**
	 * @brief SelectionKey constructor
	 */
	SelectionKey(Selector &selector, SharedPtr<Channel> channel, int events);
	
	Selector &m_selector;
	SharedPtr<Channel> m_channel;
	int m_events;
};

inline bool SelectionKey::isReadable() {
	return m_events & OP_READ;
}

inline bool SelectionKey::isWritable() {
	return m_events & OP_WRITE;
}

inline bool SelectionKey::isValid() {
	return m_channel != nullptr;
}

inline SharedPtr< Channel > SelectionKey::channel() {
	return m_channel;
}

typedef SharedPtr<SelectionKey> SelectionKeyPtr;

}

#endif // JSONPARSER_SELECTIONKEY_H
