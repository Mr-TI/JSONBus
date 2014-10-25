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

#ifndef JSONPARSER_SELECTABLECHANNEL_H
#define JSONPARSER_SELECTABLECHANNEL_H

#include <nodebus/core/shareddata.h>
#include <nodebus/core/exception.h>
#include <nodebus/core/sharedptr.h>
#include <nodebus/nio/selector.h>

/**
 * @namespace
 */
namespace NodeBus {

class SelectionKey;
class Selector;

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class Channel: public QObject, public SharedData {
	Q_OBJECT
	friend class Selector;
	friend class SelectionKey;
public:
	/**
	 * @brief Channel constructor
	 */
	Channel();
	
	/**
	 * @brief Channel destructor
	 */
	virtual ~Channel() = 0;
	
	/**
	 * @brief Register this channel to the given selector
	 * 
	 * @param selector Reference to the selector
	 * @param options SelectionKey::Flag
	 * @param attachement Pointer to an optional attachement
	 * 
	 * @return Pointer to the selection key
	 */
	SharedPtr<SelectionKey> registerTo(Selector& selector, int options, GenericPtr attachement=NULL);
	
	/**
	 * @brief Get the suitable selection key related to the given selector
	 * 
	 * @return Pointer to the selection key or null
	 */
	SharedPtr<SelectionKey> keyFor(Selector &selector);
	
protected:
	virtual void closeChannel();
	virtual int &fd() = 0;
	virtual void closeFd() = 0;
	virtual void updateStatus(int events) = 0;
	bool m_active;
	
private:
	QMap<Selector*, SharedPtr<SelectionKey> > m_keys;
};

typedef SharedPtr<Channel> ChannelPtr;

}

#endif // JSONPARSER_SELECTABLECHANNEL_H
