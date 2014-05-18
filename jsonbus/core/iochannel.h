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

#include <sys/time.h>
#include <jsonbus/core/shareddata.h>
#include <jsonbus/core/abstractchannel.h>

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
class IOChannel: public AbstractChannel {
public:
	/**
	 * @brief AbstractChannel constructor
	 */
	IOChannel(int fd);
	
	/**
	 * @brief AbstractChannel destructor
	 */
	virtual ~IOChannel();
	
	/**
	 * @brief Close the channel
	 */
	virtual void close() throw(IOException);
	
	/**
	 * @brief Get the inner file descriptor if supported
	 * @return the inner file descriptor
	 */
	virtual int getFd();
	
protected:
	virtual size_t s_available() throw(IOException);
	virtual size_t s_read(char *buffer, size_t maxlen) throw(IOException);
	virtual void s_write(const char *buffer, size_t len) throw(IOException);

private:
	int m_fd;
};

inline IOChannel::IOChannel(int fd) : m_fd(fd) {
}

inline IOChannel::~IOChannel() {
}

inline int IOChannel::getFd() {
	return m_fd;
}

}

#endif // JSONPARSER_IOCHANNEL_H
