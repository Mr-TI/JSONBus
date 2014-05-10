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

#include <unistd.h>
#include "iodevicebuf.h"

namespace jsonparser {

IODeviceBuf::IODeviceBuf(QIODevice &device)
        : m_device(device) {
}

IODeviceBuf::~IODeviceBuf() {
}

int IODeviceBuf::getNextChar() {
	if (m_device.atEnd()) {
		return EOF;
	} else {
		char c;
		m_device.getChar(&c);
		return c;
	}
}

bool IODeviceBuf::waitReadyToRead(int timeout) {
	if (m_device.bytesAvailable() == 0) {
		usleep(timeout);
		return false;
	} else {
		return true;
	}
}

}
