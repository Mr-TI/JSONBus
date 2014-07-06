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

#include "peer.h"
#include "peeradmin.h"
#include <typeinfo>
#include <nodebus/core/logger.h>
#include <nodebus/core/selectionkey.h>
#include <nodebus/core/streamchannel.h>
#include <nodebus/core/jsonparser.h>
#include <nodebus/core/jsonserializer.h>

namespace NodeBus {

Peer::Peer(SocketChannelPtr socket): m_socket(socket) {
	connect(socket.data(), SIGNAL(closed()), this, SLOT(cancel()));
}

Peer::~Peer() {
	cancel();
}

void Peer::Task::run() {
	try {
		m_peer->process();
		return;
	} catch (EOFException &e) {
		logFine() << __demangle(typeid(*this).name()) << " Peer connection closed";
	} catch (Exception &e) {
		logWarn() << __demangle(typeid(*this).name()) << " Close peer connection after throwing an instance of '" << __demangle(typeid(e).name()) << "'";
		if (!e.message().isEmpty())
			logWarn() << "  what(): " << e.message();
	}
	try {
		m_peer->cancel();
	} catch (Exception &e) {
		logWarn() << __demangle(typeid(*this).name()) << " Throwing an instance of '" << __demangle(typeid(e).name()) << "'";
		if (!e.message().isEmpty())
			logWarn() << "  what(): " << e;
	}
}

void Peer::cancel() {
	if (m_socket == nullptr) {
		return;
	}
	m_socket->close();
	m_socket = nullptr;
}

}
