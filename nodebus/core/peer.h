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

/**
 * @brief NodeBus proxy: Client
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */


#ifndef NODEBUS_PEER_H
#define NODEBUS_PEER_H

#include <nodebus/core/sharedptr.h>
#include <nodebus/core/streamchannel.h>
#include <nodebus/core/socketchannel.h>
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
