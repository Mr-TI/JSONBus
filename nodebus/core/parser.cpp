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
#include "parser.h"
#include "streamchannel.h"
#include "jsonparser/driver.h"
#include "logger.h"
#include <qt4/QtCore/QVariant>
#include <qt4/QtCore/QDate>

#define BCON_TOKEN_END		((char)0x00)
#define BCON_TOKEN_NULL		((char)0x01)
#define BCON_TOKEN_TRUE		((char)0x02)
#define BCON_TOKEN_FALSE	((char)0x03)
#define BCON_TOKEN_BYTE		((char)0x04)
#define BCON_TOKEN_INT32	((char)0x05)
#define BCON_TOKEN_INT64	((char)0x06)
#define BCON_TOKEN_UINT32	((char)0x07)
#define BCON_TOKEN_UINT64	((char)0x08)
#define BCON_TOKEN_DOUBLE	((char)0x0A)
#define BCON_TOKEN_DATETIME	((char)0x0B)
#define BCON_TOKEN_MAP		((char)0x0C)
#define BCON_TOKEN_LIST		((char)0x0D)

#define BSON_TOKEN_END		((char)0x00)
#define BSON_TOKEN_NULL		((char)0x0A)
#define BSON_TOKEN_TRUE		((char)0x00)
#define BSON_TOKEN_FALSE	((char)0x01)
#define BSON_TOKEN_INT32	((char)0x10)
#define BSON_TOKEN_INT64	((char)0x12)
#define BSON_TOKEN_DOUBLE	((char)0x01)
#define BSON_TOKEN_DATETIME	((char)0x09)
#define BSON_TOKEN_STRING	((char)0x02)
#define BSON_TOKEN_DATA		((char)0x05)
#define BSON_TOKEN_UNDEF	((char)0x06)
#define BSON_TOKEN_OID		((char)0x07)
#define BSON_TOKEN_BOOL		((char)0x08)
#define BSON_TOKEN_REGEX	((char)0x0B)
#define BSON_TOKEN_JSCODE	((char)0x0D)
#define BSON_TOKEN_DEPREC	((char)0x0E)
#define BSON_TOKEN_MAP		((char)0x03)
#define BSON_TOKEN_LIST		((char)0x04)
#define BSON_TOKEN_GENERIC	((char)0x00)
#define BSON_TOKEN_OLDUUID	((char)0x03)
#define BSON_TOKEN_UUID		((char)0x04)

namespace NodeBus {

struct __data_buffer_t {
	const char *data;
	const char *end;
};

static char __parser_datastream_getc(void* ptr) {
	if (((__data_buffer_t*)ptr)->data == ((__data_buffer_t*)ptr)->end) {
		throw EOFException();
	}
	return *(((__data_buffer_t*)ptr)->data++);
}

static char __parser_channel_getc(void* stream) {
	return ((StreamChannel*)stream)->get();
}

Parser::Parser(fGetc_t getChar, void* ptr, Format format)
	: m_format(format), m_getChar(getChar), m_ptr(ptr) {
	if (format == Parser::JSON) {
		m_ptr = new jsonparser::Driver(getChar, ptr);
	}
}

QVariant Parser::parse(const QByteArray& data, Format format) {
	__data_buffer_t buf = {data.constData(), data.constData() + data.length()};
	Parser parser(__parser_datastream_getc, &buf, format);
	return parser.parse();
}

QVariant Parser::parse(const char* data, uint len, Format format) {
	__data_buffer_t buf = {data, data + len};
	Parser parser(__parser_datastream_getc, &buf, format);
	return parser.parse();
}

Parser::Parser(StreamChannelPtr channel, Format format)
	: m_format(format), m_channel(channel), m_getChar(__parser_channel_getc), m_ptr(channel.data()) {
	if (format == Parser::JSON) {
		m_ptr = new jsonparser::Driver(m_getChar, m_ptr);
	}
}

Parser::~Parser() {
	if (m_format == Parser::JSON) {
		delete (jsonparser::Driver*)m_ptr;
	}
}

QVariant Parser::parse() {
	QVariant res;
	switch (m_format) {
		case Parser::BCON:
			parseBCON(res, NULL);
			break;
		case Parser::BSON:
			res = parseBSONDocument();
			break;
		case Parser::JSON:
			res = ((jsonparser::Driver*)(m_ptr))->parse();
			break;
	}
	return res;
}

inline char Parser::getc() {
	return m_getChar(m_ptr);
}

inline uint32_t Parser::read32() {
	return (getc() & 0xFF)
		| ((getc() & 0xFF) << 8)
		| ((getc() & 0xFF) << 16)
		| ((getc() & 0xFF) << 24);
}

inline uint64_t Parser::read64() {
	return (uint64_t)(getc() & 0xFF)
		| ((uint64_t)(getc() & 0xFF) << 8)
		| ((uint64_t)(getc() & 0xFF) << 16)
		| ((uint64_t)(getc() & 0xFF) << 24)
		| ((uint64_t)(getc() & 0xFF) << 32)
		| ((uint64_t)(getc() & 0xFF) << 40)
		| ((uint64_t)(getc() & 0xFF) << 48)
		| ((uint64_t)(getc() & 0xFF) << 56);
}

bool Parser::parseBCON(QVariant &res, QString* key) {
	char c = getc();
	if (c & 0x80) {
		int32_t len;
		len = c & 0x3F;
		char buf[len];
		for (int i = 0; i < len; i++) {
			buf[i] = getc();
		}
		res = (c & 0x40) ? QString::fromUtf8(buf, len) : QByteArray(buf, len);
	} else if (c & 0xF0) {
		int32_t len;
		len = c & 0x0F;
		switch (c & 0x30) {
			case 0x10:
				len |= (getc() & 0xFF) << 4;
				break;
			case 0x20:
				len |= ((getc() & 0xFF) << 4) 
					| ((getc() & 0xFF) << 12);
				break;
			case 0x30:
				len |= ((getc() & 0xFF) << 4) 
					| ((getc() & 0xFF) << 12) 
					| ((getc() & 0xFF) << 20);
				break;
			default:
				throw ParserException("Invalid " + QString((c & 0x40) ? "BCON_TOKEN_STRING" : "BCON_TOKEN_DATA")  + " type " + QString::number(uint8_t(c & 0x30), 16));
		}
		char buf[len];
		for (int i = 0; i < len; i++) {
			buf[i] = getc();
		}
		res = (c & 0x40) ? QString::fromUtf8(buf, len) : QByteArray(buf, len);
	} else {
		switch (c) {
			case BCON_TOKEN_END:
				return false;
			case BCON_TOKEN_NULL:
				res = QVariant();
				break;
			case BCON_TOKEN_TRUE:
				res = QVariant(true);
				break;
			case BCON_TOKEN_FALSE:
				res = QVariant(false);
				break;
			case BCON_TOKEN_BYTE:
				res = QVariant(getc());
				break;
			case BCON_TOKEN_INT32:
				res = QVariant((int32_t)read32());
				break;
			case BCON_TOKEN_INT64:
				res = QVariant((qlonglong)read64());
				break;
			case BCON_TOKEN_UINT32:
				res = QVariant((uint32_t)read32());
				break;
			case BCON_TOKEN_UINT64:
				res = QVariant((qulonglong)read64());
				break;
			case BCON_TOKEN_DOUBLE:
				res = QVariant((double)read64());
				break;
			case BCON_TOKEN_DATETIME:
				res = QVariant(QDateTime::fromMSecsSinceEpoch((qlonglong)read64()));
				break;
			case BCON_TOKEN_MAP:
			{
				QVariantMap map;
				while (true) {
					QString key;
					QVariant value;
					if (!parseBCON(value, &key)) break;
					map[key] = value;
				}
				res = map;
				break;
			}
			case BCON_TOKEN_LIST:
			{
				QVariantList list;
				while (true) {
					QVariant value;
					if (!parseBCON(value, NULL)) break;
					list.append(value);
				}
				res = list;
				break;
			}
			default:
				throw ParserException("Invalid token " + c);
		}
	}
	if (key) {
		while ((c = getc()) != '\0') {
			key->append(c);
		}
	}
	return true;
}

QVariant Parser::parseBSONDocument() {
	read32();
	QVariantMap map;
	while (true) {
		QString key;
		QVariant value;
		if (!parseBSONElt(value, key)) break;
		map[key] = value;
	}
	return map;
}

bool Parser::parseBSONElt(QVariant &res, QString &key) {
	char c, t = getc();
	if (t == BSON_TOKEN_END) {
		return false;
	}
	while ((c = getc()) != '\0') {
		key.append(c);
	}
	switch (t) {
		case BSON_TOKEN_UNDEF:
			logWarn() << "Deprecated token Undefined";
		case BSON_TOKEN_NULL:
			res = QVariant();
			break;
		case BSON_TOKEN_BOOL:
			res = QVariant(getc() == BSON_TOKEN_TRUE);
			break;
		case BSON_TOKEN_INT32:
			res = QVariant((int32_t)read32());
			break;
		case BSON_TOKEN_INT64:
			res = QVariant((qlonglong)read64());
			break;
		case BSON_TOKEN_DOUBLE:
			res = QVariant((double)read64());
			break;
		case BSON_TOKEN_DATETIME:
			res = QVariant(QDateTime::fromMSecsSinceEpoch((qlonglong)read64()));
			break;
		case BSON_TOKEN_STRING:
		case BSON_TOKEN_JSCODE:
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
		case BSON_TOKEN_OID:
		{
			int32_t len = 12;
			char buf[len];
			for (int i = 0; i < len; i++) {
				buf[i] = getc();
			}
			res = QVariant(QByteArray(buf, len));
			break;
		}
		case BSON_TOKEN_DATA:
		{
			char type = getc();
			int32_t len = read32();
			char buf[len];
			for (int i = 0; i < len; i++) {
				buf[i] = getc();
			}
			switch (type) {
				case BSON_TOKEN_OLDUUID:
				case BSON_TOKEN_UUID:
					res = QVariant(QUuid(QByteArray(buf, len)));
					break;
				case BSON_TOKEN_GENERIC:
				default:
					res = QVariant(QByteArray(buf, len));
			}
			break;
		}
		case BSON_TOKEN_MAP:
		{
			read32();
			QVariantMap map;
			while (true) {
				QString key;
				QVariant value;
				if (!parseBSONElt(value, key)) break;
				map[key] = value;
			}
			res = map;
			break;
		}
		case BSON_TOKEN_LIST:
		{
			read32();
			QVariantList list;
			while (true) {
				QString key;
				QVariant value;
				if (!parseBSONElt(value, key)) break;
				list.insert(key.toInt(), value);
			}
			res = list;
			break;
		}
		default:
			throw ParserException("Unsupported token " + QString::number(t, 16));
	}
	return true;
}

}
