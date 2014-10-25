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

#ifndef JSONPARSER_SELECTIONKEY_H
#define JSONPARSER_SELECTIONKEY_H

#include <nodebus/core/shareddata.h>
#include <nodebus/core/exception.h>
#include <nodebus/core/sharedptr.h>

#ifdef WIN32

#else //WIN32
#	include <sys/epoll.h>
#endif //WIN32

/**
 * @namespace
 */
namespace NodeBus {

class Channel;
class Selector;

/**
 * @brief SelectionKey
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class SelectionKey : public SharedData {
	friend class Channel;
	friend class Selector;
public:
	enum Flag {
		OP_READ
#ifdef WIN32

#else //WIN32
		= EPOLLIN
#endif //WIN32
		,
		OP_WRITE
#ifdef WIN32

#else //WIN32
		= EPOLLOUT
#endif //WIN32
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
	
	/**
	 * @brief Attach the given object to this key
	 * @param obj pointer
	 */
	void attach(GenericPtr obj);
	
	/**
	 * @brief Retrieves the current attachment
	 * @return pointer to the current attachment
	 */
	GenericPtr attachment();

private:
	/**
	 * @brief SelectionKey constructor
	 */
	SelectionKey(Selector &selector, SharedPtr<Channel> channel, int events, GenericPtr attachement=NULL);
	
	Selector &m_selector;
	SharedPtr<Channel> m_channel;
	int m_events;
	GenericPtr m_attachement;
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

inline void SelectionKey::attach(GenericPtr obj) {
	m_attachement = obj;
}

inline GenericPtr SelectionKey::attachment() {
	return m_attachement;
}

inline SharedPtr< Channel > SelectionKey::channel() {
	return m_channel;
}

typedef SharedPtr<SelectionKey> SelectionKeyPtr;

}

#endif // JSONPARSER_SELECTIONKEY_H
