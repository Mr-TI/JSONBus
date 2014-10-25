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
 * @brief NodeBus proxy: MainClient
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */


#ifndef HTTPPEER_H
#define HTTPPEER_H

#include <nodebus/core/sharedptr.h>
#include <nodebus/nio/streamchannel.h>
#include <nodebus/nio/peer.h>
#include <nodebus/core/global.h>
#include <QVariant>

class StdPeer;
using namespace NodeBus;

class HttpPeer: public Peer {
public:
	
	class Factory: public Peer::Factory {
	public:
		virtual SharedPtr<Peer> build(SocketChannelPtr channel);
	};
	
	/**
	 * @brief MainClient constructor.
	 * @param channel
	 */
	HttpPeer(SocketChannelPtr channel);
	
	virtual ~HttpPeer();
	
	void cancel();
	
	virtual void process();
	
	void sendResult(uint code, const QVariant& content);
	
private:
	bool m_processDone;
	SharedPtr<StdPeer> m_stdPeer;
	QString m_reqUid;
	FileFormat m_format;
};

inline SharedPtr< Peer > HttpPeer::Factory::build(SocketChannelPtr channel) {
	return new HttpPeer(channel);
}

#endif // HTTPPEER_H
