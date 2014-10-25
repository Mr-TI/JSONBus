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

#define BCON_TOKEN_END		((quint8)0x00)
#define BCON_TOKEN_NULL		((quint8)0x01)
#define BCON_TOKEN_TRUE		((quint8)0x02)
#define BCON_TOKEN_FALSE	((quint8)0x03)
#define BCON_TOKEN_BYTE		((quint8)0x04)
#define BCON_TOKEN_INT16	((quint8)0x05)
#define BCON_TOKEN_UINT16	((quint8)0x06)
#define BCON_TOKEN_INT32	((quint8)0x07)
#define BCON_TOKEN_UINT32	((quint8)0x08)
#define BCON_TOKEN_INT64	((quint8)0x09)
#define BCON_TOKEN_UINT64	((quint8)0x0A)
#define BCON_TOKEN_DOUBLE	((quint8)0x0B)
#define BCON_TOKEN_DATETIME	((quint8)0x0C)
#define BCON_TOKEN_LIST		((quint8)0x0E)
#define BCON_TOKEN_MAP		((quint8)0x0F)

#define BSON_TOKEN_END		((quint8)0x00)
#define BSON_TOKEN_NULL		((quint8)0x0A)
#define BSON_TOKEN_TRUE		((quint8)0x00)
#define BSON_TOKEN_FALSE	((quint8)0x01)
#define BSON_TOKEN_INT32	((quint8)0x10)
#define BSON_TOKEN_INT64	((quint8)0x12)
#define BSON_TOKEN_DOUBLE	((quint8)0x01)
#define BSON_TOKEN_DATETIME	((quint8)0x09)
#define BSON_TOKEN_STRING	((quint8)0x02)
#define BSON_TOKEN_DATA		((quint8)0x05)
#define BSON_TOKEN_UNDEF	((quint8)0x06)
#define BSON_TOKEN_OID		((quint8)0x07)
#define BSON_TOKEN_BOOL		((quint8)0x08)
#define BSON_TOKEN_REGEX	((quint8)0x0B)
#define BSON_TOKEN_JSCODE	((quint8)0x0D)
#define BSON_TOKEN_DEPREC	((quint8)0x0E)
#define BSON_TOKEN_MAP		((quint8)0x03)
#define BSON_TOKEN_LIST		((quint8)0x04)
#define BSON_TOKEN_GENERIC	((quint8)0x00)
#define BSON_TOKEN_OLDUUID	((quint8)0x03)
#define BSON_TOKEN_UUID		((quint8)0x04)

namespace NodeBus {

Parser::Parser(QDataStream &dataStream, FileFormat format)
: m_format(format), m_dataStream(dataStream), m_driver(NULL) {
	m_dataStream.setByteOrder(QDataStream::LittleEndian);
	if (format == FileFormat::JSON) {
		m_driver = new jsonparser::Driver(dataStream);
	}
}

QVariant Parser::parse(const QByteArray& data, FileFormat format) {
	QDataStream dataStream(data);
	return Parser(dataStream).parse();
}

QVariant Parser::parse(const char* data, uint len, FileFormat format) {
	QDataStream dataStream(QByteArray(data, len));
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
			QDataStream stream(&file);
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
