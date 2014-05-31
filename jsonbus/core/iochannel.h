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

#ifndef JSONPARSER_IOCHANNEL_H
#define JSONPARSER_IOCHANNEL_H

#include <sys/epoll.h>
#include <jsonbus/core/shareddata.h>
#include <jsonbus/core/streamchannel.h>

/**
 * @namespace
 */
namespace JSONBus {

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class IOChannel: public StreamChannel {
public:
	/**
	 * @brief AbstractChannel constructor
	 * @param fd a valid file descriptor
	 * @throw IOException on error
	 */
	IOChannel(int fd, bool closeOnDelete = false);
	
	/**
	 * @brief AbstractChannel destructor
	 */
	virtual ~IOChannel();
	
protected:
	virtual int &fd();
	virtual void closeFd();
	virtual void updateStatus(int events);
	virtual size_t s_available();
	virtual size_t s_read(char *buffer, size_t maxlen);
	virtual void s_write(const char *buffer, size_t len);
	bool s_waitForReadyRead(int timeout);
	int m_fd;
	bool m_closeOnDelete;

private:
	int m_epfd;
	epoll_event m_event;
	epoll_event m_events[1];
};

inline int &IOChannel::fd() {
	return m_fd;
}

}

#endif // JSONPARSER_IOCHANNEL_H
