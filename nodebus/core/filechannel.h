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

#ifndef JSONPARSER_FILECHANNEL_H
#define JSONPARSER_FILECHANNEL_H

#include <sys/epoll.h>
#include <nodebus/core/shareddata.h>
#include <nodebus/core/streamchannel.h>
#include <fcntl.h>

/**
 * @namespace
 */
namespace NodeBus {

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class FileChannel: public StreamChannel {
public:
	/**
	 * @brief AbstractChannel constructor
	 * @param fd a valid file descriptor
	 * @throw IOException on error
	 */
	FileChannel(const char *path, int flags, int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	
	/**
	 * @brief AbstractChannel destructor
	 */
	virtual ~FileChannel();
	
protected:
	virtual int &fd();
	virtual void closeFd();
	virtual size_t s_available();
	virtual size_t s_read(char *buffer, size_t maxlen);
	virtual void s_write(const char *buffer, size_t len);
	virtual void updateStatus(int events);
	bool s_waitForReadyRead(int timeout);
	int m_fd;
};

inline void FileChannel::updateStatus(int events) {
}

inline int &FileChannel::fd() {
	return m_fd;
}

}

#endif // JSONPARSER_FILECHANNEL_H
