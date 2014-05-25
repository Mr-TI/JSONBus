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

#ifndef JSONPARSER_SERVERSOCKETCHANNEL_H
#define JSONPARSER_SERVERSOCKETCHANNEL_H

#include <jsonbus/core/exception.h>
#include <jsonbus/core/channel.h>

/**
 * @namespace
 */
namespace JSONBus {

class SocketChannel;

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class ServerSocketChannel: public Channel {
public:
	/**
	 * @brief Socket constructor
	 */
	ServerSocketChannel(const QString &host, int port, int listenQueueSize=5);
	
	/**
	 * @brief Socket destructor
	 */
	virtual ~ServerSocketChannel();
	
	/**
	 * @brief Return if the channel is open
	 * @return true if the channel is open, otherwise false
	 */
	virtual bool isOpen();
	
	/**
	 * @brief Close the channel
	 * @throw IOException on error
	 */
	virtual void close();
	
	/**
	 * @brief Connect
	 */
	virtual SharedPtr<SocketChannel> accept();
	
protected:
	virtual int &fd();
	virtual void updateStatus(int events);
	int m_fd;
	QString m_name;
};

inline int &ServerSocketChannel::fd() {
	return m_fd;
}

inline bool ServerSocketChannel::isOpen() {
	return m_fd != -1;
}

typedef SharedPtr<ServerSocketChannel> ServerSocketChannelPtr;

}

#endif // JSONPARSER_SERVERSOCKETCHANNEL_H
