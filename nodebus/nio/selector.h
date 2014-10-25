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

#ifndef JSONPARSER_SELECTOR_H
#define JSONPARSER_SELECTOR_H

#include <nodebus/core/exception.h>
#include <nodebus/core/sharedptr.h>
#ifdef WIN32
	
#else //WIN32
#	include <sys/epoll.h>
#endif //WIN32
#include <QMap>

/**
 * @namespace
 */
namespace NodeBus {

class SelectionKey;

/**
 * @brief Selector
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
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
#ifdef WIN32
	
#else //WIN32
	int m_epfd;
	epoll_event m_event;
	epoll_event *m_events;
#endif //WIN32
	QMap<int, SharedPtr<SelectionKey> > m_keys;
	QList<SharedPtr<SelectionKey> > m_pendingKeys;
	QMutex m_synchronize;
};

inline void Selector::cancel() {
	m_enabled = false;
}

}

#endif // JSONPARSER_SELECTOR_H
