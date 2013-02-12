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

#include "common.h"
#include "jsonparser.h"
#include "jsonparser/driver.h"
#include "jsonparser/textstreambuf.h"
#include "jsonparser/exception.h"
#include "jsonparser/iodevicebuf.h"
#include "jsonparser/stdstreambuf.h"
#include "jsonparser/descriptorbuf.h"
#include <sstream>

using namespace jsonparser;
using namespace std;

namespace JSONBus {

JSONParser::JSONParser(QObject* parent)
	: QObject (parent), m_driver(NULL), m_inputStream(NULL), m_buffer(NULL) {
	m_driver = new Driver();
}

JSONParser::JSONParser(std::istream &stream, QObject* parent)
	: QObject (parent), m_driver(NULL), m_inputStream(NULL), m_buffer(NULL) {
	setupBuf(stream);
	m_driver = new Driver(m_inputStream);
}

JSONParser::JSONParser(int fd, QObject* parent)
	: QObject (parent), m_driver(NULL), m_inputStream(NULL), m_buffer(NULL) {
	setupBuf(fd);
	m_driver = new Driver(m_inputStream);
}

JSONParser::JSONParser(QTextStream &stream, QObject* parent)
	: QObject (parent), m_driver(NULL), m_inputStream(NULL), m_buffer(NULL) {
	setupBuf(stream);
	m_driver = new Driver(m_inputStream);
}

JSONParser::JSONParser(QIODevice &input, QObject* parent)
	: QObject (parent), m_driver(NULL), m_inputStream(NULL), m_buffer(NULL) {
	setupBuf(input);
	m_driver = new Driver(m_inputStream);
}

JSONParser::~JSONParser() {
	delete m_driver;
	cleanup();
}

void JSONParser::cleanup() {
	if (m_inputStream) {
		delete m_inputStream;
		m_inputStream = NULL;
	}
	if (m_buffer) {
		delete m_buffer;
		m_buffer = NULL;
	}
}

void JSONParser::setupBuf(int fd) {
	cleanup();
	m_buffer = new DescriptorBuf(fd);
	m_inputStream = new istream(m_buffer);
}

void JSONParser::setupBuf(istream& stream) {
	cleanup();
	m_buffer = new StdStreamBuf(stream);
	m_inputStream = new istream(m_buffer);
}

void JSONParser::setupBuf(QIODevice& device) {
	cleanup();
	m_buffer = new IODeviceBuf(device);
	m_inputStream = new istream(m_buffer);
}

void JSONParser::setupBuf(QTextStream& stream) {
	cleanup();
	m_buffer = new TextStreamBuf(stream);
	m_inputStream = new istream(m_buffer);
}

void JSONParser::disable() {
	m_buffer->disable();
}

QVariant JSONParser::parse() {
	return m_driver->parse();
}

QVariant JSONParser::parse(const QByteArray &data) {
	stringstream in(stringstream::in | stringstream::out);
	in << data.data();
	return m_driver->parse(&in);
}

QVariant JSONParser::parse(int fd) {
	setupBuf(fd);
	return m_driver->parse(m_inputStream);
}

QVariant JSONParser::parse(std::istream &stream) {
	setupBuf(stream);
	return m_driver->parse(m_inputStream);
}

QVariant JSONParser::parse(QTextStream &stream) {
	setupBuf(stream);
	return m_driver->parse(m_inputStream);
}

QVariant JSONParser::parse(QIODevice &input) {
	setupBuf(input);
	return m_driver->parse(m_inputStream);
}

}
