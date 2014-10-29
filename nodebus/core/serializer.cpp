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
#include "serializer.h"
#include <QVariant>

#define DEBUG_TK(t) logFinest() << (t)

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
static const quint8 BCON_TOKEN_DATA6	= 0xA0;
static const quint8 BCON_TOKEN_STRING6	= 0xC0;
static const quint8 BCON_TOKEN_DATA12	= 0x10;
static const quint8 BCON_TOKEN_DATA20	= 0x20;
static const quint8 BCON_TOKEN_DATA36	= 0x30;
static const quint8 BCON_TOKEN_STRING12	= 0x50;
static const quint8 BCON_TOKEN_STRING20	= 0x60;
static const quint8 BCON_TOKEN_STRING36	= 0x70;

#define LENGTH2P6		64
#define LENGTH2P12		4096
#define LENGTH2P20		1048576
#define LENGTH2P36		68719476736

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
static const quint8 BSON_TOKEN_BOOL	= 0x08;
static const quint8 BSON_TOKEN_MAP	= 0x03;
static const quint8 BSON_TOKEN_LIST	= 0x04;
static const quint8 BSON_TOKEN_GENERIC	= 0x00;

#define JSON_KW(s)		QString(s).toLocal8Bit()
#define JSON_NULL		JSON_KW("null")
#define JSON_TRUE		JSON_KW("true")
#define JSON_FALSE		JSON_KW("false")
#define JSON_OBJECT_BEGIN	JSON_KW("{")
#define JSON_OBJECT_END		JSON_KW("}")
#define JSON_MEMBER_SEP		JSON_KW(":")
#define JSON_ELEMENT_SEP	JSON_KW(",")
#define JSON_ARRAY_BEGIN	JSON_KW("[")
#define JSON_ARRAY_END		JSON_KW("]")

namespace NodeBus {

quint32 Serializer::FORMAT_COMPACT = 0x00000020u;


QString Serializer::toJSONString(const QVariant& variant, quint32 flags) {
	QByteArray data;
	QBuffer buffer(&data);
	DataStream dataStream(&buffer);
	serialize(dataStream, variant, JSON, flags);
	return QString::fromLocal8Bit(data);
}

void Serializer::serialize(DataStream& dataStream, const QVariant& variant, FileFormat format, uint32_t flags) {
	return Serializer(dataStream, format).serialize(variant, flags);
}

Serializer::Serializer(DataStream &dataStream, FileFormat format)
: m_dataStream(dataStream), m_format(format) {
// 	m_dataStream.setByteOrder(DataStream::LittleEndian);
}

Serializer::~Serializer() {
}

void Serializer::toFile(const QString &fileName, const QVariant &variant, FileFormat format, uint32_t flags) {
	switch (format) {
		case FileFormat::BCON:
		case FileFormat::BSON:
		case FileFormat::JSON: {
			QFile file(fileName);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
				throw IOException(file.errorString());
			}
			DataStream stream(&file);
			return Serializer(stream, format).serialize(variant, flags);
		}
		case FileFormat::IDL:
			throw Exception("Unsupported IDL format");
	}
}

void Serializer::serialize(const QVariant& variant, quint32 flags) {
	switch (m_format) {
		case FileFormat::BCON:
			serializeBCON(variant);
			break;
		case FileFormat::BSON:
			m_dataStream << serializeBSONDocument(variant);
			break;
		case FileFormat::JSON:
			serializeJSON(variant, flags);
			break;
		case FileFormat::IDL:
			throw Exception("Unsupported IDL format");
	}
}

static QString sanitizeString( QString str ) {
	str.replace( QLatin1String( "\\" ), QLatin1String( "\\\\" ) );
	str.replace( QLatin1String( "\"" ), QLatin1String( "\\\"" ) );
	str.replace( QLatin1String( "\b" ), QLatin1String( "\\b" ) );
	str.replace( QLatin1String( "\f" ), QLatin1String( "\\f" ) );
	str.replace( QLatin1String( "\n" ), QLatin1String( "\\n" ) );
	str.replace( QLatin1String( "\r" ), QLatin1String( "\\r" ) );
	str.replace( QLatin1String( "\t" ), QLatin1String( "\\t" ) );
	return QString( QLatin1String( "\"%1\"" ) ).arg( str );
}

void Serializer::serializeBCON(const QVariant &variant, const QString *key) {
	switch (variant.type()) {
		case QVariant::Invalid:
			DEBUG_TK("BCON_TOKEN_NULL");
			m_dataStream << BCON_TOKEN_NULL;
			break;
		case QVariant::Bool: // Case of BCON boolean
			DEBUG_TK(variant.toBool() ? "BCON_TOKEN_TRUE": "BCON_TOKEN_FALSE");
			m_dataStream << (variant.toBool() ? BCON_TOKEN_TRUE: BCON_TOKEN_FALSE);
			break;
		case QVariant::Char: // Case of BCON boolean
			DEBUG_TK("BCON_TOKEN_BYTE");
			m_dataStream << BCON_TOKEN_BYTE
					<< variant.toChar().toAscii();
			break;
		case QVariant::Int:
		{
			qint32 num = variant.toInt();
			if ((num & 0x7FFFFF80) == 0) {
				DEBUG_TK("BCON_TOKEN_BYTE");
				m_dataStream << BCON_TOKEN_BYTE
					<< variant.toChar().toAscii();
			} else if ((num & 0x7FFF8000) == 0) {
				DEBUG_TK("BCON_TOKEN_INT16");
				m_dataStream << BCON_TOKEN_INT16 << qint16(num);
			} else {
				DEBUG_TK("BCON_TOKEN_INT32");
				m_dataStream << BCON_TOKEN_INT32 << num;
			}
			break;
		}
		case QVariant::UInt: {
			quint32 num = variant.toUInt();
			if ((num & 0xFFFF0000u) == 0) {
				DEBUG_TK("BCON_TOKEN_UINT16");
				m_dataStream << BCON_TOKEN_UINT16 << quint16(num);
			} else {
				DEBUG_TK("BCON_TOKEN_UINT32");
				m_dataStream << BCON_TOKEN_UINT32 << num;
			}
			break;
		}
		case QVariant::LongLong:
		{
			DEBUG_TK("BCON_TOKEN_INT64");
			m_dataStream << BCON_TOKEN_INT64 << variant.toLongLong();
			break;
		}
		case QVariant::ULongLong:
		{
			DEBUG_TK("BCON_TOKEN_UINT64");
			m_dataStream << BCON_TOKEN_UINT64 << variant.toULongLong();
			break;
		}
		case QVariant::Double:
		{
			DEBUG_TK("BCON_TOKEN_DOUBLE");
			m_dataStream << BCON_TOKEN_DOUBLE << variant.toDouble();
			break;
		}
		case QVariant::DateTime:
		case QVariant::Date:
		case QVariant::Time:
		{
			DEBUG_TK("BCON_TOKEN_DATETIME");
			m_dataStream << BCON_TOKEN_DATETIME << variant.toDateTime().toMSecsSinceEpoch();
			break;
		}
		case QVariant::List: // Case of BCON array
		{
			DEBUG_TK("BCON_TOKEN_LIST");
			m_dataStream << BCON_TOKEN_LIST;
			const QVariantList elements = variant.toList();
			for (auto it = elements.begin(); it != elements.end(); it++) {
				serializeBCON(*it);
			}
			m_dataStream << BCON_TOKEN_END;
			break;
		}
		case QVariant::Map: // Case of BCON object
		{
			DEBUG_TK("BCON_TOKEN_MAP");
			m_dataStream << BCON_TOKEN_MAP;
			const QVariantMap elements = variant.toMap();
			for (auto it = elements.begin(); it != elements.end(); it++) {
				serializeBCON(it.value(), &(it.key()));
			}
			m_dataStream << BCON_TOKEN_END;
			break;
		}
		case QVariant::String:
		{
			QByteArray data = variant.toString().toLocal8Bit();
			int len = data.length();
			if (len < (LENGTH2P6)) {
				DEBUG_TK("BCON_TOKEN_STRING6");
				m_dataStream << quint8((BCON_TOKEN_STRING6) | (len & 0x3F));
				m_dataStream << data;
			} else if (len < (LENGTH2P12)) {
				DEBUG_TK("BCON_TOKEN_STRING12");
				m_dataStream << quint8((BCON_TOKEN_STRING12) | (len & 0x0F))
						<< ((len & 0xFF0) >> 4);
				m_dataStream << data;
			} else if (len < (LENGTH2P20)) {
				DEBUG_TK("BCON_TOKEN_STRING20");
				m_dataStream << quint8((BCON_TOKEN_STRING20) | (len & 0x0F))
						<< ((len & 0xFF0) >> 4)
						<< ((len & 0xFF000) >> 12);
				m_dataStream << data;
			} else if (len < (LENGTH2P36)) {
				DEBUG_TK("BCON_TOKEN_STRING36");
				m_dataStream << quint8((BCON_TOKEN_STRING36) | (len & 0x0F))
						<< ((len & 0xFF0) >> 4)
						<< ((len & 0xFF000) >> 12)
						<< (len >> 20);
				m_dataStream << data;
			} else {
				throw SerializerException("Fatal: too big String (length=" + QString::number(len) + ")");
			}
			break;
		}
		case QVariant::ByteArray: // Case of BCON string
		{
			QByteArray data = variant.toByteArray();
			int len = data.length();
			if (len < (LENGTH2P6)) {
				DEBUG_TK("BCON_TOKEN_DATA6");
				m_dataStream << quint8((BCON_TOKEN_DATA6) | (len & 0x3F));
				m_dataStream << data;
			} else if (len < (LENGTH2P12)) {
				DEBUG_TK("BCON_TOKEN_DATA12");
				m_dataStream << quint8((BCON_TOKEN_DATA12) | (len & 0x0F))
						<< ((len & 0xFF0) >> 4);
				m_dataStream << data;
			} else if (len < (LENGTH2P20)) {
				DEBUG_TK("BCON_TOKEN_DATA20");
				m_dataStream << quint8((BCON_TOKEN_DATA20) | (len & 0x0F))
						<< ((len & 0xFF0) >> 4)
						<< ((len & 0xFF000) >> 12);
				m_dataStream << data;
			} else if (len < (LENGTH2P36)) {
				DEBUG_TK("BCON_TOKEN_DATA36");
				m_dataStream << quint8((BCON_TOKEN_DATA36) | (len & 0x0F))
						<< ((len & 0xFF0) >> 4)
						<< ((len & 0xFF000) >> 12)
						<< (len >> 20);
				m_dataStream << data;
			} else {
				throw SerializerException("Fatal: too big byte array (length=" + QString::number(len) + ")");
			}
			break;
		}
		default:
			throw SerializerException("Fatal: QVariant type not managed.");
		
	}
		
	if (key) m_dataStream << (*key).toLocal8Bit() << '\0';
}

void Serializer::serializeJSON(const QVariant &variant, quint32 flags) {
	switch (variant.type()) {
		case QVariant::Invalid:
		{
			m_dataStream << JSON_NULL;
			break;
		}
		case QVariant::Bool: // Case of JSON boolean
		{
			m_dataStream << (variant.toBool() ? JSON_TRUE: JSON_FALSE);
			break;
		}
		case QVariant::Map: // Case of JSON object
		{
			bool compact = (flags & FORMAT_COMPACT) != 0;
			quint8 indentStep = INDENT(flags);
			quint32 indentOff = (flags >> 16) + indentStep;
			flags = (flags & 0xFFFF) | (indentOff << 16);
			m_dataStream << JSON_OBJECT_BEGIN;
			const QVariantMap elements = variant.toMap();
			auto it = elements.begin();
			if (it != elements.end()) {
				if (indentOff != 0) m_dataStream << '\n' << QString(indentOff, ' ');
				m_dataStream << sanitizeString(it.key()) << JSON_MEMBER_SEP;
				if (!compact) m_dataStream << ' ';
				serializeJSON(it.value(), flags);
				it++;
				while (it != elements.end()) {
					m_dataStream << JSON_ELEMENT_SEP;
					if (indentOff != 0) m_dataStream << '\n' << QString(indentOff, ' ');
					else if (!compact) m_dataStream << ' ';
					m_dataStream << sanitizeString(it.key()) << JSON_MEMBER_SEP;
					if (!compact) m_dataStream << ' ';
					serializeJSON(it.value(), flags);
					it++;
				}
				if (indentOff != 0) m_dataStream << '\n' << QString(indentOff - indentStep, ' ');
			}
			m_dataStream << JSON_OBJECT_END;
			break;
		}
		case QVariant::List: // Case of JSON array
		{
			bool compact = (flags & FORMAT_COMPACT) != 0;
			quint8 indentStep = INDENT(flags);
			quint32 indentOff = (flags >> 16) + indentStep;
			flags = (flags & 0xFFFF) | (indentOff << 16);
			m_dataStream << JSON_ARRAY_BEGIN;
			const QVariantList elements = variant.toList();
			auto it = elements.begin();
			if (it != elements.end()) {
				if (indentOff != 0) m_dataStream << '\n' << QString(indentOff, ' ');
				serializeJSON(*it, flags);
				it++;
				while (it != elements.end()) {
					m_dataStream << JSON_ELEMENT_SEP;
					if (indentOff != 0) m_dataStream << '\n' << QString(indentOff, ' ');
					else if (!compact) m_dataStream << ' ';
					serializeJSON(*it, flags);
					it++;
				}
				if (indentOff != 0) m_dataStream << '\n' << QString(indentOff - indentStep, ' ');
			}
			m_dataStream << JSON_ARRAY_END;
			break;
		}
		case QVariant::String:
		case QVariant::ByteArray: // Case of JSON string
		{
			m_dataStream << sanitizeString(variant.toString());
			break;
		}
		case QVariant::Char:
		case QVariant::Int:
		{
			m_dataStream << QString::number(variant.toInt()).replace("inf", "infinity");
			break;
		}
		case QVariant::UInt:
		{
			m_dataStream << QString::number(variant.toUInt()).replace("inf", "infinity");
			break;
		}
		case QVariant::LongLong:
		{
			m_dataStream << QString::number(variant.toLongLong()).replace("inf", "infinity");
			break;
		}
		case QVariant::ULongLong:
		{
			m_dataStream << QString::number(variant.toULongLong()).replace("inf", "infinity");
			break;
		}
		case QVariant::Double:
		{
			m_dataStream << QString::number(variant.toDouble()).replace("inf", "infinity");
			break;
		}
		default:
			throw SerializerException("Fatal: QVariant type not managed.");
	}
}

QByteArray Serializer::serializeBSONDocument(const QVariant &variant) {
	QByteArray payload;
	QBuffer payloadBuf(&payload);
    payloadBuf.open(QIODevice::WriteOnly);
	DataStream payloadDataStream(&payloadBuf);
	switch (variant.type()) {
		case QVariant::Map: // Case of BSON object
		{
			const QVariantMap elements = variant.toMap();
			for (auto it = elements.begin(); it != elements.end(); it++) {
				serializeBSONElt(payloadDataStream, it.value(), it.key());
			}
			break;
		}
		case QVariant::List: // Case of BSON array
		{
			const QVariantList elements = variant.toList();
			uint i = 0;
			for (auto it = elements.begin(); it != elements.end(); it++, i++) {
				serializeBSONElt(payloadDataStream, *it, QString::number(i));
			}
			break;
		}
		default:
			throw SerializerException("Fatal: Invalid document.");
	}
	QByteArray ret;
	QBuffer retBuf(&ret);
    retBuf.open(QIODevice::WriteOnly);
	DataStream retDataStream(&retBuf);
	retDataStream << qint32(payload.length() + 5) << payload << BSON_TOKEN_END;
	return ret;
}

void Serializer::serializeBSONElt(DataStream &dataStream, const QVariant& variant, const QString &key) {
	switch (variant.type()) {
		case QVariant::Invalid:
			DEBUG_TK("BSON_TOKEN_NULL");
			dataStream << BSON_TOKEN_NULL << key.toLocal8Bit() << '\0';
			break;
		case QVariant::Bool: // Case of BSON boolean
			DEBUG_TK("BSON_TOKEN_BOOL");
			dataStream << BSON_TOKEN_BOOL << key.toLocal8Bit() << '\0' << (variant.toBool() ? BSON_TOKEN_TRUE: BSON_TOKEN_FALSE);
			break;
		case QVariant::UInt:
		case QVariant::Int:
		case QVariant::Char:
		{
			DEBUG_TK("BSON_TOKEN_INT32");
			dataStream << BSON_TOKEN_INT32 << key.toLocal8Bit() << '\0' << variant.toUInt();
			break;
		}
		case QVariant::ULongLong:
		case QVariant::LongLong:
		{
			DEBUG_TK("BSON_TOKEN_INT64");
			dataStream << BSON_TOKEN_INT64 << key.toLocal8Bit() << '\0' << variant.toUInt();
			break;
		}
		case QVariant::Double:
		{
			DEBUG_TK("BSON_TOKEN_DOUBLE");
			dataStream << BSON_TOKEN_DOUBLE << key.toLocal8Bit() << '\0' << variant.toDouble();
			break;
		}
		case QVariant::DateTime:
		case QVariant::Date:
		case QVariant::Time:
		{
			DEBUG_TK("BSON_TOKEN_DATETIME");
			dataStream << BSON_TOKEN_DATETIME << key.toLocal8Bit() << '\0' << variant.toDateTime().toMSecsSinceEpoch();
			break;
		}
		case QVariant::Map: // Case of BSON object
		{
			DEBUG_TK("BSON_TOKEN_MAP");
			dataStream << BSON_TOKEN_MAP << key.toLocal8Bit() << '\0' << serializeBSONDocument(variant);
			break;
		}
		case QVariant::List: // Case of BSON array
		{
			DEBUG_TK("BSON_TOKEN_LIST");
			dataStream << BSON_TOKEN_LIST << key.toLocal8Bit() << '\0' << serializeBSONDocument(variant);
			break;
		}
		case QVariant::String:
		{
			DEBUG_TK("BSON_TOKEN_STRING");
			QByteArray data = variant.toString().toLocal8Bit();
			dataStream << BSON_TOKEN_STRING << key.toLocal8Bit() << '\0' << data.length() + 1 << data << '\0';
			break;
		}
		case QVariant::ByteArray: // Case of BSON string
		{
			DEBUG_TK("BSON_TOKEN_DATA");
			QByteArray data = variant.toByteArray();
			dataStream << BSON_TOKEN_DATA << key.toLocal8Bit() << '\0' << data.length() << BSON_TOKEN_GENERIC << data;
			break;
		}
		default:
			throw SerializerException("Fatal: QVariant type not managed.");
	}
}

}
