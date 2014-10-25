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

#include "peeradmin.h"
#include <typeinfo>
#include <qt4/QtCore/qthreadpool.h>
#include <nodebus/core/logger.h>
#include <nodebus/nio/selectionkey.h>
#include <nodebus/core/parser.h>

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
