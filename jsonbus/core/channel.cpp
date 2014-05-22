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
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + __LINE__ + ": " + strerror(errno))

namespace JSONBus {

Channel::Channel() {
}

Channel::~Channel() {
}

SharedPtr< SelectionKey > Channel::registerTo(Selector& sel, int ops) {
	QMutexLocker _(&(sel.m_synchronize));
	SelectionKeyPtr key = new SelectionKey(sel, this);
	sel.put(key, ops & (SelectionKey::OP_READ | SelectionKey::OP_WRITE));
	m_keys[&sel] = key;
	return key;
}

}
