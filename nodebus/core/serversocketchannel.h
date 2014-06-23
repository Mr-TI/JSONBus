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

#include <nodebus/core/exception.h>
#include <nodebus/core/channel.h>

/**
 * @namespace
 */
namespace NodeBus {

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
	
	enum {
		OPT_REUSEPORT = 0x00010000,
		OPT_REUSEADDR = 0x00020000,
		MASK_OPTIONS = 0xFFFF0000,
		MASK_BACKLOG = 0x0000FFFF
	};
	
	static uint OPT_BACKLOG(uint pendingQueueMaxLen);
	
	/**
	 * @brief Socket constructor
	 */
	ServerSocketChannel(const QString &host, int port, uint opts=OPT_BACKLOG(5));
	
	/**
	 * @brief Socket destructor
	 */
	virtual ~ServerSocketChannel();
	
	/**
	 * @brief Connect
	 */
	virtual SharedPtr<SocketChannel> accept();
	
	void setKeepAlive(bool value);
	void setKeepIntlv(int value);
	void setKeepIdle(int value);
	void setKeepCnt(int value);
	
protected:
	/**
	 * @brief Connect
	 */
	virtual void s_accept(int &cldf, QString &name);
	
	virtual int &fd();
	virtual void closeFd();
	virtual void updateStatus(int events);
	int m_fd;
	QString m_name;
	int m_keepAlive;
	int m_keepIntlv;
	int m_keepIdle;
	int m_keepCnt;
};

inline uint ServerSocketChannel::OPT_BACKLOG(uint pendingQueueMaxLen) {
	return pendingQueueMaxLen & MASK_BACKLOG;
}

inline int &ServerSocketChannel::fd() {
	return m_fd;
}

inline void ServerSocketChannel::setKeepAlive(bool value) {
	m_keepAlive = value ? 1 : 0;
}

inline void ServerSocketChannel::setKeepCnt(int value) {
	m_keepCnt = value;
}

inline void ServerSocketChannel::setKeepIdle(int value) {
	m_keepIdle = value;
}

inline void ServerSocketChannel::setKeepIntlv(int value) {
	m_keepIntlv = value;
}


typedef SharedPtr<ServerSocketChannel> ServerSocketChannelPtr;

}

#endif // JSONPARSER_SERVERSOCKETCHANNEL_H
