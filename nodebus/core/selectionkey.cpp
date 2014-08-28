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

#include "selectionkey.h"
#include "logger.h"
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
