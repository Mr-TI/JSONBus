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

/**
 * @brief NodeBus proxy: Client
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */


#ifndef NODEBUS_PEER_H
#define NODEBUS_PEER_H

#include <nodebus/core/sharedptr.h>
#include <nodebus/nio/streamchannel.h>
#include <nodebus/nio/socketchannel.h>
#include <qt4/QtCore/QRunnable>
#include <QVariant>

namespace NodeBus {

class Peer: public QObject, public SharedData {
	Q_OBJECT
public:
	
	class Factory: public SharedData {
	public:
		virtual SharedPtr<Peer> build(SocketChannelPtr channel) = 0;
	};
	
	class Task: public QRunnable {
	public:
		Task(SharedPtr<Peer> peer);
		virtual void run();
	private:
		SharedPtr<Peer> m_peer;
	};
	
	/**
	 * @brief Peer constructor.
	 * @param socket
	 */
	Peer(SocketChannelPtr socket);
	
	virtual ~Peer();
	
	virtual void process() = 0;
	
	bool isActive();
	
public slots:
	virtual void cancel();
	
protected:
	SocketChannelPtr m_socket;
};

typedef SharedPtr<Peer> PeerPtr;

inline Peer::Task::Task(SharedPtr< Peer > peer): m_peer(peer) {
}

inline bool Peer::isActive() {
	return m_socket != nullptr && m_socket->isOpen();
}

}

#endif // NODEBUS_PEER_H
