/*
 * Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "common.h"
#include "parser.h"
#include "jsonparser/driver.h"
#include "logger.h"
#include "idlparser/driver.h"
#include <qt4/QtCore/QVariant>
#include <qt4/QtCore/QDate>

static const quint8 BCON_TOKEN_END	= 0x00;
static const quint8 BCON_TOKEN_NULL	= 0x01;
static const quint8 BCON_TOKEN_TRUE	= 0x02;
static const quint8 BCON_TOKEN_FALSE	= 0x03;
static const quint8 BCON_TOKEN_BYTE	= 0x04;
static const quint8 BCON_TOKEN_INT16	= 0x05;
static const quint8 BCON_TOKEN_UINT16	= 0x06;
static const quint8 BCON_TOKEN_INT32	= 0x07;
static const quint8 BCON_TOKEN_UINT32	= 0x08;
static const quint8 BCON_TOKEN_INT64	= 0x09;
static const quint8 BCON_TOKEN_UINT64	= 0x0A;
static const quint8 BCON_TOKEN_DOUBLE	= 0x0B;
static const quint8 BCON_TOKEN_DATETIME	= 0x0C;
static const quint8 BCON_TOKEN_LIST	= 0x0E;
static const quint8 BCON_TOKEN_MAP	= 0x0F;

static const quint8 BSON_TOKEN_END	= 0x00;
static const quint8 BSON_TOKEN_NULL	= 0x0A;
static const quint8 BSON_TOKEN_TRUE	= 0x00;
static const quint8 BSON_TOKEN_FALSE	= 0x01;
static const quint8 BSON_TOKEN_INT32	= 0x10;
static const quint8 BSON_TOKEN_INT64	= 0x12;
static const quint8 BSON_TOKEN_DOUBLE	= 0x01;
static const quint8 BSON_TOKEN_DATETIME	= 0x09;
static const quint8 BSON_TOKEN_STRING	= 0x02;
static const quint8 BSON_TOKEN_DATA	= 0x05;
static const quint8 BSON_TOKEN_UNDEF	= 0x06;
static const quint8 BSON_TOKEN_OID	= 0x07;
static const quint8 BSON_TOKEN_BOOL	= 0x08;
static const quint8 BSON_TOKEN_REGEX	= 0x0B;
static const quint8 BSON_TOKEN_JSCODE	= 0x0D;
static const quint8 BSON_TOKEN_DEPREC	= 0x0E;
static const quint8 BSON_TOKEN_MAP	= 0x03;
static const quint8 BSON_TOKEN_LIST	= 0x04;
static const quint8 BSON_TOKEN_GENERIC	= 0x00;
static const quint8 BSON_TOKEN_OLDUUID	= 0x03;
static const quint8 BSON_TOKEN_UUID	= 0x04;

namespace NodeBus {

Parser::Parser(DataStream &dataStream, FileFormat format)
: m_format(format), m_dataStream(dataStream), m_driver(NULL) {
// 	m_dataStream.setByteOrder(QDataStream::LittleEndian);
	if (format == FileFormat::JSON) {
		m_driver = new jsonparser::Driver(dataStream);
	}
}

QVariant Parser::parse(const QByteArray& data, FileFormat format) {
	QByteArray byteArray(data);
	QBuffer buf(&byteArray);
	DataStream dataStream(&buf);
	return Parser(dataStream).parse();
}

QVariant Parser::parse(const char* data, uint len, FileFormat format) {
	QByteArray byteArray(data, len);
	QBuffer buf(&byteArray);
	DataStream dataStream(&buf);
	return Parser(dataStream).parse();
}

Parser::~Parser() {
	if (m_format == FileFormat::JSON) {
		delete (jsonparser::Driver*)m_driver;
	}
}

QVariant Parser::parse() {
	QVariant res;
	switch (m_format) {
		case FileFormat::BCON:
			parseBCON(res, NULL);
			break;
		case FileFormat::BSON:
			res = parseBSONDocument();
			break;
		case FileFormat::JSON:
			res = ((jsonparser::Driver*)(m_driver))->parse();
			break;
		case FileFormat::IDL:
			throw Exception("Unsupported IDL format");
// 			break;
	}
	return res;
}

QVariant Parser::fromFile(const QString &fileName, FileFormat format) {
	switch (format) {
		case FileFormat::BCON:
		case FileFormat::BSON:
		case FileFormat::JSON: {
			QFile file(fileName);
			if (!file.open(QIODevice::ReadOnly)) {
				throw IOException(file.errorString());
			}
			DataStream stream(&file);
			return Parser(stream, format).parse();
		}
		case FileFormat::IDL:
			return idlparser::Driver::parse(fileName);
	}
	throw ParserException();
}

template <typename T>
inline T Parser::read() {
	T value;
	m_dataStream >> value;
	return value;
}

bool Parser::parseBCON(QVariant &res, QString* key) {
	char c = read<quint8>();
	if (c & 0x80) {
		qint32 len;
		len = c & 0x3F;
		char buf[len];
		for (int i = 0; i < len; i++) {
			buf[i] = read<quint8>();
		}
		res = (c & 0x40) ? QString::fromUtf8(buf, len) : QByteArray(buf, len);
	} else if (c & 0xF0) {
		qint32 len;
		len = c & 0x0F;
		switch (c & 0x30) {
			case 0x10:
				len |= read<quint8>() << 4;
				break;
			case 0x20:
				len |= (read<quint8>() << 4) 
					| (read<quint8>() << 12);
				break;
			case 0x30:
				len |= (read<quint8>() << 4) 
					| (read<quint8>() << 12) 
					| (read<quint8>() << 20);
				break;
			default:
				throw ParserException("Invalid " + QString((c & 0x40) ? "BCON_TOKEN_STRING" : "BCON_TOKEN_DATA")  + " type " + QString::number(quint8(c & 0x30), 16));
		}
		char buf[len];
		for (int i = 0; i < len; i++) {
			buf[i] = read<quint8>();
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
				res = QVariant(read<quint8>());
				break;
			case BCON_TOKEN_INT16:
				res = QVariant(read<qint16>());
				break;
			case BCON_TOKEN_UINT16:
				res = QVariant(read<quint16>());
				break;
			case BCON_TOKEN_INT32:
				res = QVariant(read<qint32>());
				break;
			case BCON_TOKEN_UINT32:
				res = QVariant(read<quint32>());
				break;
			case BCON_TOKEN_INT64:
				res = QVariant(read<qlonglong>());
				break;
			case BCON_TOKEN_UINT64:
				res = QVariant(read<qulonglong>());
				break;
			case BCON_TOKEN_DOUBLE:
				res = QVariant(read<double>());
				break;
			case BCON_TOKEN_DATETIME:
				res = QVariant(QDateTime::fromMSecsSinceEpoch(read<qlonglong>()));
				break;
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
			default:
				throw ParserException("Invalid token " + c);
		}
	}
	if (key) {
		while ((c = read<quint8>()) != '\0') {
			key->append(c);
		}
	}
	return true;
}

QVariant Parser::parseBSONDocument() {
	read<quint32>();
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
	char c, t = read<quint8>();
	if (t == BSON_TOKEN_END) {
		return false;
	}
	while ((c = read<quint8>()) != '\0') {
		key.append(c);
	}
	switch (t) {
		case BSON_TOKEN_UNDEF:
			logWarn() << "Deprecated token Undefined";
		case BSON_TOKEN_NULL:
			res = QVariant();
			break;
		case BSON_TOKEN_BOOL:
			res = QVariant(read<quint8>() == BSON_TOKEN_TRUE);
			break;
		case BSON_TOKEN_INT32:
			res = QVariant(read<qint32>());
			break;
		case BSON_TOKEN_INT64:
			res = QVariant(read<qlonglong>());
			break;
		case BSON_TOKEN_DOUBLE:
			res = QVariant(read<double>());
			break;
		case BSON_TOKEN_DATETIME:
			res = QVariant(QDateTime::fromMSecsSinceEpoch(read<qlonglong>()));
			break;
		case BSON_TOKEN_STRING:
		case BSON_TOKEN_JSCODE:
		{
			quint32 len = read<quint32>()-1;
			char buf[len];
			for (quint32 i = 0; i < len; i++) {
				buf[i] = read<quint8>();
			}
			read<quint8>();
			res = QVariant(QString::fromUtf8(buf, len));
			break;
		}
		case BSON_TOKEN_OID:
		{
			quint32 len = 12;
			char buf[len];
			for (quint32 i = 0; i < len; i++) {
				buf[i] = read<quint8>();
			}
			res = QVariant(QByteArray(buf, len));
			break;
		}
		case BSON_TOKEN_DATA:
		{
			char type = read<quint8>();
			quint32 len = read<quint32>();
			char buf[len];
			for (quint32 i = 0; i < len; i++) {
				buf[i] = read<quint8>();
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
			read<quint32>();
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
			read<quint32>();
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
