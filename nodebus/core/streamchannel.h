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

#include <nodebus/core/exception.h>
#include <nodebus/core/channel.h>

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
class StreamChannel: public Channel {
public:
	/**
	 * @brief AbstractChannel constructor
	 */
	StreamChannel();
	
	/**
	 * @brief AbstractChannel destructor
	 */
	virtual ~StreamChannel() = 0;
	
	/**
	 * @brief Close the channel
	 * @throw IOException on error
	 */
	virtual void close();
	
	/**
	 * @brief Get the next char
	 * @return a byte
	 * @throw IOException on error
	 */
	char get();
	
	/**
	 * @brief Return the next char without extract it
	 * @return a char
	 * @throw IOException on error
	 */
	char peek();
	
	/**
	 * @brief Ignore one or more char
	 * @throw IOException on error
	 */
	void ignore(size_t len = 1);
	
	/**
	 * @brief Read a buffer
	 * @return The number of read charracters
	 * @throw IOException on error
	 */
	size_t read(char *buffer, size_t maxlen);
	
	/**
	 * @brief Write a buffer
	 * @throw IOException on error
	 */
	void write(const char *buffer, size_t len);
	
	/**
	 * @brief Flush output data
	 * @throw IOException on error
	 */
	void flush();
	
	/**
	 * @brief Get available data for read
	 * @return number of byte ready to read
	 * @throw IOException on error
	 */
	size_t available(bool noEmpty=false);
	
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
	
	char m_readBuff[64];
	size_t m_readStart;
	size_t m_readEnd;
	qint64 m_deadline;
};

inline StreamChannel::StreamChannel(): m_readStart(0), m_readEnd(0), m_deadline(-1) {
}

inline StreamChannel::~StreamChannel() {
}

inline void StreamChannel::setDeadLine(qint64 msecs) {
	m_deadline = msecs;
}

inline void StreamChannel::flush() {
}


typedef SharedPtr<StreamChannel> StreamChannelPtr;

}

#endif // JSONPARSER_ABSTRACTCHANNEL_H
