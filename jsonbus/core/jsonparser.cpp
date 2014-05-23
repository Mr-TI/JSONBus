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

#include "common.h"
#include "jsonparser.h"
#include "jsonparser/driver.h"
#include "jsonparser/exception.h"
#include "streamchannel.h"
#include <sstream>

using namespace jsonparser;

namespace JSONBus {

char JSONParser::s_getc(void* stream) {
	return ((StreamChannel*)stream)->get();
}

JSONParser::JSONParser(const StreamChannelPtr& channel)
	: m_channel(channel), m_driver(new Driver(s_getc, m_channel.data())){
}

JSONParser::~JSONParser() {
	delete m_driver;
}

QVariant JSONParser::parse(int timeout) {
	m_channel->setDeadLine(timeout == -1? -1: QDateTime::currentMSecsSinceEpoch() + timeout);
	return m_driver->parse();
}

void JSONParser::cancel() {
	m_channel->close();
}


}
