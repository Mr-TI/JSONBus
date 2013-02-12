/*
 *   Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
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

#ifndef JSONPARSER_ABSTRACTSTREAMBUF_H
#define JSONPARSER_ABSTRACTSTREAMBUF_H

#include <iostream>
#include <fstream>

/**
 * @namespace
 */
namespace jsonparser {

/**
 * @brief Abstract stream buffer
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */
class AbstractStreamBuf : public std::streambuf {
public:
	/**
	 * @brief AbstractStreamBuf constructor
	 */
	AbstractStreamBuf();
	
	/**
	 * @brief AbstractStreamBuf destructor
	 */
	virtual ~AbstractStreamBuf();
	
	/**
	 * @brief Disable the buffer
	 */
	void disable() { m_disable = true; };
	
	/**
	 * @brief underflow
	 * @return a charracter or EOF
	 */
	virtual int underflow();
	
	/**
	 * @brief uflow
	 * @return a charracter or EOF
	 */
	virtual int uflow();
protected:
	/**
	 * @brief Wait for read
	 * @param timeout Timeout in µs
	 * @return true if the buffer is ready, otherwise false
	 */
	virtual bool waitReadyToRead(int timeout=1000) = 0;
	
	/**
	 * @brief Get the next character from the buffer.
	 * @return a charracter or EOF
	 */
	virtual int getNextChar() = 0;
protected:
	bool m_disable;
	int m_curChar;
	bool m_initialized;
};

}

#endif // JSONPARSER_ABSTRACTSTREAMBUF_H
