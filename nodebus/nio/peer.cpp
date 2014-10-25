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

#include "peer.h"
#include "peeradmin.h"
#include <typeinfo>
#include <nodebus/core/logger.h>
#include <nodebus/nio/selectionkey.h>
#include <nodebus/nio/streamchannel.h>

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
