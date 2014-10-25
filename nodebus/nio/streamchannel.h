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

#ifndef JSONPARSER_ABSTRACTCHANNEL_H
#define JSONPARSER_ABSTRACTCHANNEL_H

#include <nodebus/core/exception.h>
#include <nodebus/nio/channel.h>

/**
 * @namespace
 */
namespace NodeBus {

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */
class StreamChannel: public Channel, public QIODevice {
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
