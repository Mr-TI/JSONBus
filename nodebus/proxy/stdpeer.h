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
 * @brief NodeBus proxy: Standard client
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */


#ifndef STDCLIENT_H
#define STDCLIENT_H

#include <nodebus/core/sharedptr.h>
#include <nodebus/nio/streamchannel.h>
#include <nodebus/core/parser.h>
#include <nodebus/core/serializer.h>
#include <nodebus/nio/peer.h>
#include <QVariant>
using namespace NodeBus;

class HttpPeer;

class StdPeer: public Peer {
public:
	
	class Factory: public Peer::Factory {
	private:
		FileFormat m_format;
	public:
		Factory(FileFormat format);
		virtual ~Factory();
		virtual SharedPtr<Peer> build(SocketChannelPtr socket);
	};
	
	/**
	 * @brief MainClient constructor.
	 * @param socket
	 */
	StdPeer(SocketChannelPtr socket, FileFormat format);
	
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
	QDataStream m_dataStream;
	Parser m_parser;
	Serializer m_serializer;
	QString m_uid;
	QMutex m_synchronize;
	QMap<QString, SharedPtr<HttpPeer> > m_httpPeers;
};

inline StdPeer::Factory::Factory(FileFormat format): m_format(format) {
}

inline StdPeer::Factory::~Factory() {
}

inline SharedPtr< Peer > StdPeer::Factory::build(SocketChannelPtr channel) {
	return new StdPeer(channel, m_format);
}

inline QList< QString > StdPeer::getUidList() {
	return m_stdPeers.keys();
}

inline uint StdPeer::getCount() {
	return m_stdPeers.size();
}

#endif // STDCLIENT_H
