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
 * @brief NodeBus proxy: Standard client
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */


#ifndef STDCLIENT_H
#define STDCLIENT_H

#include <nodebus/core/sharedptr.h>
#include <nodebus/core/streamchannel.h>
#include <nodebus/core/parser.h>
#include <nodebus/core/serializer.h>
#include <nodebus/core/peer.h>
#include <QVariant>
using namespace NodeBus;

class HttpPeer;

class StdPeer: public Peer {
public:
	
	class Factory: public Peer::Factory {
	public:
		virtual SharedPtr<Peer> build(SocketChannelPtr socket);
	};
	
	/**
	 * @brief MainClient constructor.
	 * @param socket
	 */
	StdPeer(SocketChannelPtr socket);
	
	virtual ~StdPeer();
	
	QString send(QVariantMap& request, SharedPtr< HttpPeer > peer);
	
	void rmRef(const QString &reqUid);
	
	virtual void cancel();
	
	virtual void process();
	
	static SharedPtr<StdPeer> get(const QString &uid);
	static void clearClientList();
	static QList<QString> getUidList();
	static uint getCount();
private:
	void writeError(const QString& object, const QString& message, const QString& type="message");
	void writeResponse(const QString &object, const QVariant &data);
	static QMap<QString, SharedPtr<StdPeer> > m_stdPeers;
	Parser m_parser;
	Serializer m_serializer;
	QString m_uid;
	QMutex m_synchronize;
	QMap<QString, SharedPtr<HttpPeer> > m_httpPeers;
};

inline SharedPtr< Peer > StdPeer::Factory::build(SocketChannelPtr channel) {
	return new StdPeer(channel);
}

inline QList< QString > StdPeer::getUidList() {
	return m_stdPeers.keys();
}

inline uint StdPeer::getCount() {
	return m_stdPeers.size();
}

#endif // STDCLIENT_H
