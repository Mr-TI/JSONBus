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

#include "descriptorbuf.h"
#include <sys/select.h>

namespace jsonparser {

DescriptorBuf::DescriptorBuf(int fd)
		: m_fd(fd) {
}

DescriptorBuf::~DescriptorBuf() {
}

int DescriptorBuf::getNextChar() {
	char c;
	if (read(m_fd, &c, 1) == 1)
		return c;
	else
		return EOF;
}

bool DescriptorBuf::waitReadyToRead(int timeout) {
	fd_set fdset;
	timeval tv = {timeout / 1000000, timeout % 1000000};
	FD_ZERO(&fdset);
	FD_SET(m_fd, &fdset);
	return select(m_fd + 1, &fdset, NULL, NULL, &tv) == 1 ? true : false;
}

}
