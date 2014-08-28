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

#include "peeradmin.h"
#include <typeinfo>
#include <qt4/QtCore/qthreadpool.h>
#include <nodebus/core/logger.h>
#include <nodebus/core/selectionkey.h>
#include "parser.h"

namespace NodeBus {

PeerAdmin::PeerAdmin(QObject* parent)
: QThread(parent), m_enabled(true), m_selector() {

}

PeerAdmin::~PeerAdmin() {
}

void PeerAdmin::run() {
	try {
		while (m_enabled) {
			if (m_selector.select() && m_enabled) {
				auto list = m_selector.selectedKeys();
				for(auto it = list.begin(); it != list.end(); it++) {
					SelectionKeyPtr key = *it;
					if (!key->isValid()) {
						continue;
					}
					if (key->channel().instanceof<ServerSocketChannel>()) {
						processServer(key->channel(), key->attachment());
					} else if (key->channel().instanceof<SocketChannel>()) {
						processPeer(key->attachment());
					}
				}
			}
		}
	} catch (ParserException &e) {
		if (m_enabled) {
			logFine() << __demangle(typeid(*this).name()) << " leaving main loop normally";
		} else {
			logCrit() << __demangle(typeid(*this).name()) << " leaving main loop after throwing an instance of '" << __demangle(typeid(e).name()) << "'";
			if (!e.message().isEmpty())
				logCrit() << "  what(): " << e.message();
		}
	} catch (Exception &e) {
		logCrit() << __demangle(typeid(*this).name()) << " leaving main loop after throwing an instance of '" << __demangle(typeid(e).name()) << "'";
		if (!e.message().isEmpty())
			logCrit() << "  what(): " << e.message();
	}
	cancel();
}

void PeerAdmin::cancel() {
	m_enabled = false;
	//StdPeer::clearClientList();
	m_selector.cancel();
}

void PeerAdmin::attach(ChannelPtr channel, GenericPtr attachement) {
	channel->registerTo(m_selector, SelectionKey::OP_READ, attachement);
}

void PeerAdmin::processPeer(PeerPtr peer) {
	if (peer->isActive()) {
		QThreadPool::globalInstance()->start(new Peer::Task(peer));
	}
}

void PeerAdmin::processServer(ServerSocketChannelPtr socket, SharedPtr< Peer::Factory > factory) {
	try {
		SocketChannelPtr clientSocket = socket->accept();
		clientSocket->registerTo(m_selector, SelectionKey::OP_READ, factory->build(clientSocket));
	} catch (Exception &e) {
		logWarn() << __demangle(typeid(*this).name()) << " Close peer connection after throwing an instance of '" << __demangle(typeid(e).name()) << "'";
		if (!e.message().isEmpty())
			logWarn() << "  what(): " << e.message();
	}
	socket->registerTo(m_selector, SelectionKey::OP_READ, factory);
}

}
