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

#include "channel.h"
#include <nodebus/core/logger.h>
#include "selector.h"
#include "selectionkey.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(strerror(errno)))

namespace NodeBus {

Channel::Channel(): m_active(true) {
}

Channel::~Channel() {
	auto list = m_keys.values();
	for (auto it = list.begin(); it != list.end(); it++) {
		(*it)->cancel();
	}
}

void Channel::closeChannel() {
	if (!m_active) {
		return;
	}
	m_active = false;
	auto list = m_keys.values();
	for (auto it = list.begin(); it != list.end(); it++) {
		(*it)->cancel();
	}
	closeFd();
}

SelectionKeyPtr Channel::registerTo(Selector& selector, int options, GenericPtr attachement) {
	if (m_keys.contains(&selector)) {
		m_keys[&selector]->cancel();
	}
	return new SelectionKey(selector, this, options, attachement);
}

SharedPtr< SelectionKey > Channel::keyFor(Selector& selector) {
	if (m_keys.contains(&selector)) {
		return m_keys[&selector];
	}
	return nullptr;
}

}
