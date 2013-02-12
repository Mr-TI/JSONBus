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

#include "abstractstreambuf.h"

namespace jsonparser {

AbstractStreamBuf::AbstractStreamBuf()
        : m_disable(false), m_initialized(false) {
}

AbstractStreamBuf::~AbstractStreamBuf() {
}

int AbstractStreamBuf::underflow() {
	if (m_disable) return EOF;
	if (!m_initialized) {
		m_initialized = true;
		uflow();
	}
	return m_curChar;
}

int AbstractStreamBuf::uflow() {
	if (!m_initialized) {
		m_initialized = true;
		uflow();
	}
	if (m_disable || m_curChar == EOF) return EOF;
	int prevChar = m_curChar;
	while (!waitReadyToRead()) {
		if (m_disable) return EOF;
	}
	m_curChar = getNextChar();
	return prevChar;
}


}
