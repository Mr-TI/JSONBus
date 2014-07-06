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
 * @brief NodeBus proxy : JSON task parser management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */


#ifndef JSONBUS_JSONPARSERTASK_H
#define JSONBUS_JSONPARSERTASK_H

#include <QRunnable>
#include <nodebus/core/jsonparser.h>
#include <nodebus/core/serversocketchannel.h>
#include <nodebus/core/socketchannel.h>
#include <nodebus/core/selectionkey.h>
#include <nodebus/core/peer.h>
using namespace NodeBus;

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

#endif // JSONBUS_JSONPARSERTASK_H
