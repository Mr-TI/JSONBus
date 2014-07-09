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
#include "csonparser.h"
#include "streamchannel.h"
#include "logger.h"
#include <qt4/QtCore/QVariant>
#include <qt4/QtCore/QDate>

#define TEND		((char)0x00)
#define TNULL		((char)0x01)
#define TTRUE		((char)0x02)
#define TFALSE		((char)0x03)
#define TBYTE		((char)0x04)
#define TINT32		((char)0x05)
#define TINT64		((char)0x06)
#define TUINT32		((char)0x07)
#define TUINT64		((char)0x08)
#define TDOUBLE		((char)0x0A)
#define TTIMESTAMP	((char)0x0B)
#define TMAP		((char)0x0C)
#define TLIST		((char)0x0D)

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

CSONParser::CSONParser(fGetc_t getChar, void* ptr)
	: m_getChar(getChar), m_ptr(ptr) {
}

QVariant CSONParser::parse(const QByteArray& data) {
	__data_buffer_t buf = {data.constData(), data.constData() + data.length()};
	CSONParser parser(jsonparser_datastream_getc, &buf);
	return parser.parse();
}

QVariant CSONParser::parse(const char* data, uint len) {
	__data_buffer_t buf = {data, data + len};
	CSONParser parser(jsonparser_datastream_getc, &buf);
	return parser.parse();
}

CSONParser::CSONParser(StreamChannelPtr channel)
	: m_getChar(jsonparser_channel_getc), m_ptr(channel.data()) {
}

CSONParser::~CSONParser() {
}

QVariant CSONParser::parse() {
	QVariant res;
	parse(res, NULL);
	return res;
}

inline char CSONParser::getc() {
	return m_getChar(m_ptr);
}

inline uint32_t CSONParser::read32() {
	return (getc() & 0xFF)
		| ((getc() & 0xFF) << 8)
		| ((getc() & 0xFF) << 16)
		| ((getc() & 0xFF) << 24);
}

inline uint64_t CSONParser::read64() {
	return (uint64_t)(getc() & 0xFF)
		| ((uint64_t)(getc() & 0xFF) << 8)
		| ((uint64_t)(getc() & 0xFF) << 16)
		| ((uint64_t)(getc() & 0xFF) << 24)
		| ((uint64_t)(getc() & 0xFF) << 32)
		| ((uint64_t)(getc() & 0xFF) << 40)
		| ((uint64_t)(getc() & 0xFF) << 48)
		| ((uint64_t)(getc() & 0xFF) << 56);
}

bool CSONParser::parse(QVariant &res, QString* key) {
	char c = getc();
	if (c & 0x80) {
		int32_t len;
		len = c & 0x3F;
		char buf[len];
		for (int i = 0; i < len; i++) {
			buf[i] = getc();
		}
		logFiner() << ((c & 0x40) ? "TSTRING6" : "TDATA6") << " len=" << len;
		res = (c & 0x40) ? QString(QByteArray(buf, len)) : QByteArray(buf, len);
	} else if (c & 0xF0) {
		int32_t len;
		len = c & 0x0F;
		switch (c & 0x30) {
			case 1:
				len |= (getc() & 0xFF) << 4;
				break;
			case 2:
				len |= ((getc() & 0xFF) << 4) 
					| ((getc() & 0xFF) << 12);
				break;
			case 3:
				len |= ((getc() & 0xFF) << 4) 
					| ((getc() & 0xFF) << 12) 
					| ((getc() & 0xFF) << 20);
				break;
		}
		char buf[len];
		for (int i = 0; i < len; i++) {
			buf[i] = getc();
		}
		res = (c & 0x40) ? QString(QByteArray(buf, len)) : QByteArray(buf, len);
	} else {
		switch (c) {
			case TEND:
				logFiner() << "TEND";
				return false;
			case TNULL:
				logFiner() << "TNULL";
				res = QVariant();
				break;
			case TTRUE:
				logFiner() << "TTRUE";
				res = QVariant(true);
				break;
			case TFALSE:
				logFiner() << "TFALSE";
				res = QVariant(false);
				break;
			case TBYTE:
				logFiner() << "TBYTE";
				res = QVariant(getc());
				break;
			case TINT32:
				logFiner() << "TINT32";
				res = QVariant((int32_t)read32());
				break;
			case TINT64:
				logFiner() << "TINT64";
				res = QVariant((qlonglong)read64());
				break;
			case TUINT32:
				logFiner() << "TUINT32";
				res = QVariant((uint32_t)read32());
				break;
			case TUINT64:
				logFiner() << "TUINT64";
				res = QVariant((qulonglong)read64());
				break;
			case TDOUBLE:
				logFiner() << "TDOUBLE";
				res = QVariant((double)read64());
				break;
			case TTIMESTAMP:
				logFiner() << "TTIMESTAMP";
				res = QVariant(QDateTime::fromMSecsSinceEpoch((qlonglong)read64()));
				break;
			case TMAP:
			{
				logFiner() << "TMAP";
				QVariantMap map;
				while (true) {
					QString key;
					QVariant value;
					if (!parse(value, &key)) break;
					map[key] = value;
				}
				res = map;
				break;
			}
			case TLIST:
			{
				logFiner() << "TLIST";
				QVariantList list;
				while (true) {
					QVariant value;
					if (!parse(value, NULL)) break;
					list.append(value);
				}
				res = list;
				break;
			}
		}
	}
	if (key) {
		while ((c = getc()) != '\0') {
			key->append(c);
		}
		logFiner() << "KEY=" << *key;
	}
	return true;
}

}
