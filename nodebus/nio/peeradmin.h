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
 * @brief NodeBus proxy : JSON task parser management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */


#ifndef NODEBUS_PEERADMIN_H
#define NODEBUS_PEERADMIN_H

#include <QRunnable>
#include <nodebus/nio/serversocketchannel.h>
#include <nodebus/nio/socketchannel.h>
#include <nodebus/nio/selectionkey.h>
#include <nodebus/nio/peer.h>

namespace NodeBus {

class PeerAdmin : public QThread {
	Q_OBJECT
public:
	
	/**
	 * @brief Server constructor.
	 * @param fd File descriptor
	 * @param parent Parent object
	 */
	PeerAdmin(QObject* parent=0);
	
	virtual ~PeerAdmin();
	
	/**
	 * @brief Task main
	 */
	virtual void run();
	
	void attach(ChannelPtr channel, GenericPtr attachement=NULL);
	void processPeer(PeerPtr client);
	
public slots:
	void cancel();
private:
	void processServer(ServerSocketChannelPtr socket, SharedPtr<Peer::Factory> factory);
	
	bool m_enabled;
	Selector m_selector;
};

}

#endif // NODEBUS_PEERADMIN_H
