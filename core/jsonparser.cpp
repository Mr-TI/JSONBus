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

namespace JSONBus {

JSONParser::JSONParser(QObject* parent)
	: QObject (parent) {
	m_handle = new jsonparser::Driver();
}

JSONParser::~JSONParser() {
	delete static_cast<jsonparser::Driver*>(m_handle);
}

QVariant JSONParser::parse() {
	QVariant result;
	try {
		result = static_cast<jsonparser::Driver*>(m_handle)->parse();
	} catch(jsonparser::Exception e) {
		throw JSONParserException(e.message());
	}
	return result;
}

QVariant JSONParser::parse(const QByteArray &data) {
	QVariant result;
	try {
		stringstream in(stringstream::in | stringstream::out);
		in << data.data();
		result = static_cast<jsonparser::Driver*>(m_handle)->parse(&in);
	} catch(jsonparser::Exception e) {
		throw JSONParserException(e.message());
	}
	return result;
}

QVariant JSONParser::parse(std::istream &stream) {
	QVariant result;
	try {
		result = static_cast<jsonparser::Driver*>(m_handle)->parse(&stream);
	} catch(jsonparser::Exception e) {
		throw JSONParserException(e.message());
	}
	return result;
}

QVariant JSONParser::parse(QTextStream &stream) {
	QVariant result;
	try {
		jsonparser::TextStreamBuf buf(stream);
		istream in(&buf);
		result = static_cast<jsonparser::Driver*>(m_handle)->parse(&in);
	} catch(jsonparser::Exception e) {
		throw JSONParserException(e.message());
	}
	return result;
}

QVariant JSONParser::parse(QIODevice &input) {
	QVariant result;
	try {
		jsonparser::IODeviceBuf buf(input);
		istream in(&buf);
		result = static_cast<jsonparser::Driver*>(m_handle)->parse(&in);
	} catch(jsonparser::Exception e) {
		throw JSONParserException(e.message());
	}
	return result;
}

}
