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

#ifndef JSONPARSER_ABSTRACTCHANNEL_H
#define JSONPARSER_ABSTRACTCHANNEL_H

#include <iostream>
#include <fstream>
#include <jsonbus/core/shareddata.h>
#include <jsonbus/core/exception.h>
#include "sharedptr.h"

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
class AbstractChannel: public SharedData {
public:
	/**
	 * @brief AbstractChannel constructor
	 */
	AbstractChannel();
	
	/**
	 * @brief AbstractChannel destructor
	 */
	virtual ~AbstractChannel() = 0;
	
	/**
	 * @brief Close the channel
	 */
	virtual void close();
	
	/**
	 * @brief Get the next char
	 * @return a byte
	 */
	char get();
	
	/**
	 * @brief Return the next char without extract it
	 * @return a char
	 */
	char peek();
	
	/**
	 * @brief Ignore one or more char
	 */
	void ignore(size_t len = 1);
	
	/**
	 * @brief Read a buffer
	 * @return The number of read charracters
	 */
	size_t read(char *buffer, size_t maxlen);
	
	/**
	 * @brief Write a buffer
	 */
	void write(const char *buffer, size_t len);
	
	/**
	 * @brief Flush output data
	 */
	void flush();
	
	/**
	 * @brief Get available data for read
	 * @return number of byte ready to read
	 */
	size_t available(bool noEmpty=false);
	
	/**
	 * @brief Get the inner file descriptor if supported
	 * @return the inner file descriptor or -1 if not supported
	 */
	virtual int getFd();
	
	/**
	 * @brief Set deadline.
	 * 
	 * This is a way to perform a timeout over several read call.
	 * This deadline can be moved or cancelled by another call.
	 * 
	 * @param msec number of milliseconds since 1970-01-01T00:00:00 
	 * Universal Coordinated Time or -1 to cancell it
	 */
	void setDeadLine(qint64 msecs);
	
protected:
	virtual size_t s_available() = 0;
	virtual size_t s_read(char *buffer, size_t maxlen) = 0;
	virtual void s_write(const char *buffer, size_t len) = 0;
	virtual bool s_waitForReadyRead(int timeout) = 0;
	
private:
	void checkBuf();
	
	bool m_enabled;
	char m_readBuff[64];
	size_t m_readStart;
	size_t m_readEnd;
	qint64 m_deadline;
};

inline AbstractChannel::AbstractChannel(): m_enabled(true), m_readStart(0), m_readEnd(0), m_deadline(-1) {
}

inline AbstractChannel::~AbstractChannel() {
}

inline void AbstractChannel::close() {
	m_enabled = false;
}

inline void AbstractChannel::setDeadLine(qint64 msecs) {
	m_deadline = msecs;
}

inline int AbstractChannel::getFd() {
	return -1;
}

inline void AbstractChannel::flush() {
}


typedef SharedPtr<AbstractChannel> ChannelPtr;

}

#endif // JSONPARSER_ABSTRACTCHANNEL_H
