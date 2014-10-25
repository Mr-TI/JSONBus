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

#include "selectionkey.h"
#include <nodebus/core/logger.h>
#include "selector.h"
#include "channel.h"

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(strerror(errno)))

namespace NodeBus {

SelectionKey::SelectionKey(Selector &selector, SharedPtr<Channel> channel, int events, GenericPtr attachement)
: m_selector(selector), m_channel(channel), m_events(0), m_attachement(attachement) {
	QMutexLocker _(&(selector.m_synchronize));
	selector.put(this, events & (SelectionKey::OP_READ | SelectionKey::OP_WRITE));
	channel->m_keys[&selector] = this;
}

SelectionKey::~SelectionKey() {
}

void SelectionKey::cancel() {
	if (!isValid()) {
		return;
	}
	QMutexLocker _(&(m_selector.m_synchronize));
	m_selector.remove(this);
	m_channel->m_keys.remove(&m_selector);
	m_channel = nullptr;
}
	
}
