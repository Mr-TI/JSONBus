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

#include "channel.h"
#include "logger.h"
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

Channel::Channel(): m_open(true) {
}

Channel::~Channel() {
	auto list = m_keys.values();
	for (auto it = list.begin(); it != list.end(); it++) {
		(*it)->cancel();
	}
}

void Channel::close() {
	if (!m_open) {
		return;
	}
	m_open = false;
	auto list = m_keys.values();
	for (auto it = list.begin(); it != list.end(); it++) {
		(*it)->cancel();
	}
	closeFd();
	emit closed();
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
