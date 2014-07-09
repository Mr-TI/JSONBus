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
#include "bsonparser.h"
#include "streamchannel.h"
#include "logger.h"
#include <qt4/QtCore/QVariant>
#include <qt4/QtCore/QDate>

#define TEND		((char)0x00)
#define TNULL		((char)0x0A)
#define TTRUE		((char)0x00)
#define TFALSE		((char)0x01)
#define TINT32		((char)0x10)
#define TINT64		((char)0x12)
#define TDOUBLE		((char)0x01)
#define TDATETIME	((char)0x09)
#define TSTRING		((char)0x02)
#define TDATA		((char)0x05)
#define TUNDEF		((char)0x06)
#define TOID		((char)0x07)
#define TBOOL		((char)0x08)
#define TREGEX		((char)0x0B)
#define TJSCODE		((char)0x0D)
#define TDEPREC		((char)0x0E)
#define TMAP		((char)0x03)
#define TLIST		((char)0x04)
#define TGENERIC	((char)0x00)
#define TOLDUUID	((char)0x03)
#define TUUID		((char)0x04)

namespace NodeBus {

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

BSONParser::BSONParser(fGetc_t getChar, void* ptr)
	: m_getChar(getChar), m_ptr(ptr) {
}

QVariant BSONParser::parse(const QByteArray& data) {
	__data_buffer_t buf = {data.constData(), data.constData() + data.length()};
	BSONParser parser(jsonparser_datastream_getc, &buf);
	return parser.parse();
}

QVariant BSONParser::parse(const char* data, uint len) {
	__data_buffer_t buf = {data, data + len};
	BSONParser parser(jsonparser_datastream_getc, &buf);
	return parser.parse();
}

BSONParser::BSONParser(StreamChannelPtr channel)
	: m_getChar(jsonparser_channel_getc), m_ptr(channel.data()) {
}

BSONParser::~BSONParser() {
}

QVariant BSONParser::parse() {
	return parseDocument();
}

inline char BSONParser::getc() {
	return m_getChar(m_ptr);
}

inline int32_t BSONParser::read32() {
	return (getc() & 0xFF)
		| ((getc() & 0xFF) << 8)
		| ((getc() & 0xFF) << 16)
		| ((getc() & 0xFF) << 24);
}

inline int64_t BSONParser::read64() {
	return (int64_t)(getc() & 0xFF)
		| ((int64_t)(getc() & 0xFF) << 8)
		| ((int64_t)(getc() & 0xFF) << 16)
		| ((int64_t)(getc() & 0xFF) << 24)
		| ((int64_t)(getc() & 0xFF) << 32)
		| ((int64_t)(getc() & 0xFF) << 40)
		| ((int64_t)(getc() & 0xFF) << 48)
		| ((int64_t)(getc() & 0xFF) << 56);
}

QVariant BSONParser::parseDocument() {
	read32();
	QVariantMap map;
	while (true) {
		QString key;
		QVariant value;
		if (!parse(value, key)) break;
		map[key] = value;
	}
	return map;
}

bool BSONParser::parse(QVariant &res, QString &key) {
	char c, t = getc();
	while ((c = getc()) != '\0') {
		key.append(c);
	}
	switch (t) {
		case TEND:
			return false;
		case TUNDEF:
			logWarn() << "Deprecated token Undefined";
		case TNULL:
			res = QVariant();
			break;
		case TBOOL:
			res = QVariant(getc() == TTRUE);
			break;
		case TINT32:
			res = QVariant((int32_t)read32());
			break;
		case TINT64:
			res = QVariant((qlonglong)read64());
			break;
		case TDOUBLE:
			res = QVariant((double)read64());
			break;
		case TDATETIME:
			res = QVariant(QDateTime::fromMSecsSinceEpoch((qlonglong)read64()));
			break;
		case TSTRING:
		case TJSCODE:
		{
			int32_t len = read32()-1;
			char buf[len];
			for (int i = 0; i < len; i++) {
				buf[i] = getc();
			}
			getc();
			res = QVariant(QString::fromUtf8(buf, len));
			break;
		}
		case TOID:
		{
			int32_t len = 12;
			char buf[len];
			for (int i = 0; i < len; i++) {
				buf[i] = getc();
			}
			res = QVariant(QByteArray(buf, len));
			break;
		}
		case TDATA:
		{
			char type = getc();
			int32_t len = read32();
			char buf[len];
			for (int i = 0; i < len; i++) {
				buf[i] = getc();
			}
			switch (type) {
				case TOLDUUID:
				case TUUID:
					res = QVariant(QUuid(QByteArray(buf, len)));
					break;
				case TGENERIC:
				default:
					res = QVariant(QByteArray(buf, len));
			}
			break;
		}
		case TMAP:
		{
			QVariantMap map;
			while (true) {
				QString key;
				QVariant value;
				if (!parse(value, key)) break;
				map[key] = value;
			}
			res = map;
			break;
		}
		case TLIST:
		{
			QVariantList list;
			while (true) {
				QString key;
				QVariant value;
				if (!parse(value, key)) break;
				list.insert(key.toInt(), value);
			}
			res = list;
			break;
		}
		default:
			throw BSONParserException("Unsupported token " + t);
	}
	return true;
}

}
