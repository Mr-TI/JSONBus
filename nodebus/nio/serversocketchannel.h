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

#ifndef JSONPARSER_SERVERSOCKETCHANNEL_H
#define JSONPARSER_SERVERSOCKETCHANNEL_H

#include <nodebus/core/exception.h>
#include <nodebus/nio/channel.h>

/**
 * @namespace
 */
namespace NodeBus {

class SocketChannel;

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
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
