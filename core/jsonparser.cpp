/*
    Copyright (c) 2012, Emeric Verschuur <contact@openihs.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the OpenIHS.org nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "common.h"
#include "jsonparser.h"
#include "jsonparser/driver.h"
#include "jsonparser/textstreambuf.h"
#include "jsonparser/exception.h"
#include "jsonparser/iodevicebuf.h"
#include <sstream>

using namespace jsonparser;

namespace JSONBus {

JSONParser::JSONParser(QObject* parent)
	: QObject (parent), m_driver(NULL), m_inputStream(NULL), m_devBuf(NULL), m_tsBuf(NULL) {
	m_driver = new Driver();
}

JSONParser::JSONParser(std::istream &stream, QObject* parent)
	: QObject (parent), m_driver(NULL), m_inputStream(NULL), m_devBuf(NULL), m_tsBuf(NULL) {
	m_driver = new Driver(&stream);
}

JSONParser::JSONParser(QTextStream &stream, QObject* parent)
	: QObject (parent), m_driver(NULL), m_inputStream(NULL), m_devBuf(NULL), m_tsBuf(NULL) {
	setupTextStreamBuf(stream);
	m_driver = new Driver(m_inputStream);
}

JSONParser::JSONParser(QIODevice &input, QObject* parent)
	: QObject (parent), m_driver(NULL), m_inputStream(NULL), m_devBuf(NULL), m_tsBuf(NULL) {
	setupIODeviceBuf(input);
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
	if (m_devBuf) {
		delete m_devBuf;
		m_devBuf = NULL;
	}
	if (m_tsBuf) {
		delete m_tsBuf;
		m_tsBuf = NULL;
	}
}


void JSONParser::setupIODeviceBuf(QIODevice& device) {
	cleanup();
	m_devBuf = new IODeviceBuf(device);
	m_inputStream = new istream(m_devBuf);
}

void JSONParser::setupTextStreamBuf(QTextStream& stream) {
	cleanup();
	m_tsBuf = new TextStreamBuf(stream);
	m_inputStream = new istream(m_devBuf);
}

QVariant JSONParser::parse() {
	return m_driver->parse();
}

QVariant JSONParser::parse(const QByteArray &data) {
	stringstream in(stringstream::in | stringstream::out);
	in << data.data();
	return m_driver->parse(&in);
}

QVariant JSONParser::parse(std::istream &stream) {
	return m_driver->parse(&stream);
}

QVariant JSONParser::parse(QTextStream &stream) {
	setupTextStreamBuf(stream);
	return m_driver->parse(m_inputStream);
}

QVariant JSONParser::parse(QIODevice &input) {
	setupIODeviceBuf(input);
	return m_driver->parse(m_inputStream);
}

}
