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

#ifndef JSONPARSER_SELECTOR_H
#define JSONPARSER_SELECTOR_H

#include <jsonbus/core/exception.h>
#include <jsonbus/core/sharedptr.h>
#include <sys/epoll.h>
#include <QMap>

/**
 * @namespace
 */
namespace JSONBus {

class SelectionKey;
class Channel;

/**
 * @brief Selector
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class Selector {
	friend class SelectionKey;
	friend class Channel;
public:
	/**
	 * @brief Selector constructor
	 */
	Selector();
	
	/**
	 * @brief Selector destructor
	 */
	virtual ~Selector();
	
	/**
	 * @brief Select the ready channels for respertive designed operations
	 * @param timeout time in milliseconds or -1 for an undefined time
	 */
	virtual bool select(int timeout = -1);
	
	/**
	 * @brief Get selected keys
	 * @param return the key list
	 */
	QList< SharedPtr<SelectionKey> > selectedKeys();
	
	/**
	 * @brief Cancel the selector
	 */
	virtual void cancel();

private:
	
	/**
	 * @brief Register or update a channel to this selector
	 * @param channel channel to register or update
	 * @param flags
	 */
	virtual void put(const SharedPtr<SelectionKey> &key, int events);
	
	/**
	 * @brief Remove a channel from this selector
	 * @param channel channel to remove
	 */
	virtual void remove(const SharedPtr<SelectionKey> &key);
	
	bool m_enabled;
	int m_epfd;
	epoll_event m_event;
	epoll_event m_events[64];
	QMap<int, SharedPtr<SelectionKey> > m_keys;
	QList<SharedPtr<SelectionKey> > m_pendingKeys;
	QMutex m_synchronize;
};

inline void Selector::cancel() {
	m_enabled = false;
}

}

#endif // JSONPARSER_SELECTOR_H
