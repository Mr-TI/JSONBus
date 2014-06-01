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

struct __data_buffer_t {
	const char *data;
	const char *end;
};

static char jsonparser_datastream_getc(void* ptr) {
	if (((__data_buffer_t*)ptr)->data == ((__data_buffer_t*)ptr)->end) {
		throw EOFException();
	}
	return *(((__data_buffer_t*)ptr)->data++);
}

static char jsonparser_channel_getc(void* stream) {
	return ((StreamChannel*)stream)->get();
}

JSONParser::JSONParser(fGetc_t getChar, void* ptr)
	: m_driver(new Driver(getChar, ptr)) {
}

QVariant JSONParser::parse(const QString& data) {
	QByteArray tmp = data.toUtf8();
	__data_buffer_t buf = {tmp.constData(), tmp.constData() + tmp.length()};
	JSONParser parser(jsonparser_datastream_getc, &buf);
	return parser.parse();
}

QVariant JSONParser::parse(const char* data, uint len) {
	__data_buffer_t buf = {data, data + len};
	JSONParser parser(jsonparser_datastream_getc, &buf);
	return parser.parse();
}

JSONParser::JSONParser(StreamChannelPtr channel)
	: m_driver(new Driver(jsonparser_channel_getc, channel.data())) {
}

JSONParser::~JSONParser() {
	delete m_driver;
}

QVariant JSONParser::parse() {
	return m_driver->parse();
}

}
