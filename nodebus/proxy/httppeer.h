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
 * @brief NodeBus proxy: MainClient
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */


#ifndef HTTPPEER_H
#define HTTPPEER_H

#include <nodebus/core/sharedptr.h>
#include <nodebus/core/streamchannel.h>
#include <nodebus/core/peer.h>
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
};

inline SharedPtr< Peer > HttpPeer::Factory::build(SocketChannelPtr channel) {
	return new HttpPeer(channel);
}

#endif // HTTPPEER_H
