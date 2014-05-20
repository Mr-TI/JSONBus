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

#include "selector.h"
#include "selectionkey.h"
#include "logger.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <QString>

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + __LINE__ + ": " + strerror(errno))

namespace JSONBus {

Selector::Selector() : m_eventCnt(0) {
	THROW_IOEXP_ON_ERR(m_epfd = epoll_create1(0));
}

Selector::~Selector() {
	::close(m_epfd);
}

bool Selector::select(int timeout) {
	ssize_t ret;
	THROW_IOEXP_ON_ERR(ret = epoll_wait(m_epfd, m_events,sizeof(m_events) ,timeout));
	return (m_eventCnt = ret) > 0;
}

QList< SelectionKeyPtr > Selector::selectedKeys() {
	QList< SelectionKeyPtr > list;
	for (uint i = 0; i < m_eventCnt; i++) {
		if (!m_keys.contains(m_events[i].data.fd)) {
			continue;
		}
		SelectionKeyPtr key = m_keys[m_events[i].data.fd];
		m_keys.remove(m_events[i].data.fd);
		key->m_events = m_events[i].events;
		list.append(key);
	}
	return list;
}


void Selector::put(const SharedPtr< SelectionKey >& key) {
	m_keys[key->channel()->s_fd()] = key;
}

void Selector::remove(SelectionKey* key) {

}

}
