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

#ifndef JSONPARSER_STDSTREAMBUF_H
#define JSONPARSER_STDSTREAMBUF_H

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <QIODevice>
#include "abstractstreambuf.h"

/**
 * @namespace
 */
namespace jsonparser {

/**
 * @brief STD stream based stream buffer
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class StdStreamBuf : public AbstractStreamBuf {
public:
	inline StdStreamBuf(std::istream &stream): m_streambuf(*stream.rdbuf()) {};
	inline virtual ~StdStreamBuf() {};
protected:
	inline virtual bool waitReadyToRead(int timeout) { 
		if (!m_streambuf.in_avail()) {
			usleep(timeout);
			return false;
		} else {
			return true;
		}
	};
	inline virtual int getNextChar() { return m_streambuf.sbumpc(); };
	
private:
	std::streambuf &m_streambuf;
};

}

#endif // JSONPARSER_STDSTREAMBUF_H
