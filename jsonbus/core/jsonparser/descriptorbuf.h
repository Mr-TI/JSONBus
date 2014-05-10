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

#ifndef JSONPARSER_DESCRIPTORBUF_H
#define JSONPARSER_DESCRIPTORBUF_H

#include "abstractstreambuf.h"
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * @namespace
 */
namespace jsonparser {

/**
 * @brief File descriptor based stream buffer
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class DescriptorBuf : public AbstractStreamBuf {
public:
	DescriptorBuf(int fd);
	virtual ~DescriptorBuf();
protected:
	virtual bool waitReadyToRead(int timeout);
	virtual int getNextChar();
private:
	int m_fd;
	
};

}

#endif // JSONPARSER_DESCRIPTORBUF_H
