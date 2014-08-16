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
#include "serializer.h"
#include <qt4/QtCore/QVariant>

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
#define BCON_TOKEN_DATA6	((char)0xA0)
#define BCON_TOKEN_STRING6	((char)0xC0)
#define BCON_TOKEN_DATA12	((char)0x10)
#define BCON_TOKEN_DATA20	((char)0x20)
#define BCON_TOKEN_DATA36	((char)0x30)
#define BCON_TOKEN_STRING12	((char)0x50)
#define BCON_TOKEN_STRING20	((char)0x60)
#define BCON_TOKEN_STRING36	((char)0x70)

#define LENGTH2P6	64
#define LENGTH2P12	4096
#define LENGTH2P20	1048576
#define LENGTH2P36	68719476736

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
#define BSON_TOKEN_BOOL		((char)0x08)
#define BSON_TOKEN_MAP		((char)0x03)
#define BSON_TOKEN_LIST		((char)0x04)
#define BSON_TOKEN_GENERIC	((char)0x00)

#define JSON_NULL			QString("null")
#define JSON_TRUE			QString("true")
#define JSON_FALSE			QString("false")
#define JSON_OBJECT_BEGIN	QString("{")
#define JSON_OBJECT_END		QString("}")
#define JSON_MEMBER_SEP		QString(":")
#define JSON_ELEMENT_SEP	QString(",")
#define JSON_ARRAY_BEGIN	QString("[")
#define JSON_ARRAY_END		QString("]")

namespace NodeBus {

/**
	* @brief ouput stream
	*/
class SerializerChannelOutputStream :public Serializer::OutputStream {
public:
	/**
	 * @brief ouput stream constructor from an std::ostream
	 * @param stream std::ostream reference
	 */
	SerializerChannelOutputStream (const StreamChannelPtr& channel);
	
	/**
	 * @brief ouput stream destructor
	 */
	~SerializerChannelOutputStream ();
	
	/**
	 * @brief output stream operator
	 */
	OutputStream& operator << (char byte);
	
	/**
	 * @brief output stream operator
	 */
	OutputStream& operator << (const QByteArray &data);
	
	/**
	 * @brief output stream operator
	 */
	OutputStream& operator << (const QString &data);
private:
	StreamChannelPtr m_channel;
};

inline SerializerChannelOutputStream::SerializerChannelOutputStream(const StreamChannelPtr& channel): m_channel(channel) {
}

inline SerializerChannelOutputStream::~SerializerChannelOutputStream() {
}

inline Serializer::OutputStream& SerializerChannelOutputStream::operator<<(char byte) {
	m_channel->write(&byte, 1);
	return *this; 
}

inline Serializer::OutputStream& SerializerChannelOutputStream::operator<<(const QByteArray& data) {
	m_channel->write(data.data(), data.length());
	return *this; 
}

inline Serializer::OutputStream& SerializerChannelOutputStream::operator<<(const QString& data) {
	QByteArray bytes = data.toUtf8();
	m_channel->write(bytes.data(), bytes.length());
	return *this; 
}

/**
	* @brief Std ouput stream
	*/
class SerializerByteArrayOutputStream :public Serializer::OutputStream {
public:
	/**
		* @brief Std ouput stream constructor from an std::ostream
		* @param stream std::ostream reference
		*/
	SerializerByteArrayOutputStream (QByteArray& data);
	
	/**
		* @brief Std ouput stream destructor
		*/
	~SerializerByteArrayOutputStream();
	
	/**
	 * @brief output stream operator
	 */
	OutputStream& operator << (char byte);
	
	/**
	 * @brief output stream operator
	 */
	OutputStream& operator << (const QByteArray &data);
	
	/**
	 * @brief output stream operator
	 */
	OutputStream& operator << (const QString &data);
private:
	QByteArray& m_data;
};

SerializerByteArrayOutputStream::SerializerByteArrayOutputStream(QByteArray& data): m_data(data) {
}

SerializerByteArrayOutputStream::~SerializerByteArrayOutputStream() {
}

inline Serializer::OutputStream& SerializerByteArrayOutputStream::operator<<(char byte) {
	m_data.append(byte);
	return *this;
}

inline Serializer::OutputStream& SerializerByteArrayOutputStream::operator<<(const QByteArray& data) {
	m_data.append(data);
	return *this;
}

Serializer::OutputStream& SerializerByteArrayOutputStream::operator<<(const QString& data) {
	m_data.append(data);
	return *this; 
}

QString Serializer::toJSONString(const QVariant& variant) {
	QByteArray data;
	Serializer(data).serialize(variant);
	return QString::fromLocal8Bit(data);
}

Serializer::Serializer(StreamChannelPtr channel, FileFormat format)
: m_streamPtr(new SerializerChannelOutputStream(channel)), m_stream(*m_streamPtr), m_format(format) {
	
}

Serializer::Serializer(QByteArray& data, FileFormat format)
: m_streamPtr(new SerializerByteArrayOutputStream(data)), m_stream(*m_streamPtr), m_format(format) {
}

Serializer::Serializer(Serializer::OutputStream& stream, FileFormat format)
: m_stream(stream), m_format(format) {
	
}

Serializer::~Serializer() {
}

void Serializer::serialize(const QVariant& variant) {
	switch (m_format) {
		case FileFormat::BCON:
			serializeBCON(variant);
			break;
		case FileFormat::BSON:
			m_stream << serializeBSONDocument(variant);
			break;
		case FileFormat::JSON:
			serializeJSON(variant);
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

inline void Serializer::write32(char type, uint32_t value) {
	m_stream << type
		<< ((char)((value & 0xFF)))
		<< ((char)(((value >> 8) & 0xFF)))
		<< ((char)(((value >> 16) & 0xFF)))
		<< ((char)(((value >> 24) & 0xFF)));
}

inline void Serializer::write64(char type, uint64_t value) {
	m_stream << type
		<< ((char)((value & 0xFF)))
		<< ((char)(((value >> 8) & 0xFF)))
		<< ((char)(((value >> 16) & 0xFF)))
		<< ((char)(((value >> 24) & 0xFF)))
		<< ((char)(((value >> 32) & 0xFF)))
		<< ((char)(((value >> 40) & 0xFF)))
		<< ((char)(((value >> 48) & 0xFF)))
		<< ((char)(((value >> 56) & 0xFF)));
}

void Serializer::serializeBCON(const QVariant &variant, const QString *key) {
	switch (variant.type()) {
		case QVariant::Invalid:
			m_stream << BCON_TOKEN_NULL;
			break;
		case QVariant::Bool: // Case of BCON boolean
			m_stream << (variant.toBool() ? BCON_TOKEN_TRUE: BCON_TOKEN_FALSE);
			break;
		case QVariant::Char: // Case of BCON boolean
			m_stream << BCON_TOKEN_BYTE
					<< variant.toChar().toAscii();
			break;
		case QVariant::UInt: {
			write32(BCON_TOKEN_UINT32, variant.toUInt());
			break;
		}
		case QVariant::ULongLong:
		{
			write64(BCON_TOKEN_UINT64, variant.toUInt());
			break;
		}
		case QVariant::Int:
		{
			write32(BCON_TOKEN_INT32, variant.toUInt());
			break;
		}
		case QVariant::LongLong:
		{
			write64(BCON_TOKEN_INT64, variant.toUInt());
			break;
		}
		case QVariant::Double:
		{
			write64(BCON_TOKEN_DOUBLE, variant.toDouble());
			break;
		}
		case QVariant::DateTime:
		case QVariant::Date:
		case QVariant::Time:
		{
			write64(BCON_TOKEN_DATETIME, variant.toDateTime().toMSecsSinceEpoch());
			break;
		}
		case QVariant::Map: // Case of BCON object
		{
			m_stream << BCON_TOKEN_MAP;
			const QVariantMap elements = variant.toMap();
			for (auto it = elements.begin(); it != elements.end(); it++) {
				serializeBCON(it.value(), &(it.key()));
			}
			m_stream << BCON_TOKEN_END;
			break;
		}
		case QVariant::List: // Case of BCON array
		{
			m_stream << BCON_TOKEN_LIST;
			const QVariantList elements = variant.toList();
			for (auto it = elements.begin(); it != elements.end(); it++) {
				serializeBCON(*it);
			}
			m_stream << BCON_TOKEN_END;
			break;
		}
		case QVariant::String:
		{
			QByteArray data = variant.toString().toLocal8Bit();
			int len = data.length();
			if (len < (LENGTH2P6)) {
				m_stream << (char) ((BCON_TOKEN_STRING6 & 0xFF) | (len & 0x3F));
				m_stream << data;
			} else if (len < (LENGTH2P12)) {
				m_stream << (char) ((BCON_TOKEN_STRING12 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4);
				m_stream << data;
			} else if (len < (LENGTH2P20)) {
				m_stream << (char) ((BCON_TOKEN_STRING20 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4)
						<< (char) ((len & 0xFF000) >> 12);
				m_stream << data;
			} else if (len < (LENGTH2P36)) {
				m_stream << (char) ((BCON_TOKEN_STRING36 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4)
						<< (char) ((len & 0xFF000) >> 12)
						<< (char) (len >> 20);
				m_stream << data;
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
				m_stream << (char) ((BCON_TOKEN_DATA6 & 0xFF) | (len & 0x3F));
				m_stream << data;
			} else if (len < (LENGTH2P12)) {
				m_stream << (char) ((BCON_TOKEN_DATA12 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4);
				m_stream << data;
			} else if (len < (LENGTH2P20)) {
				m_stream << (char) ((BCON_TOKEN_DATA20 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4)
						<< (char) ((len & 0xFF000) >> 12);
				m_stream << data;
			} else if (len < (LENGTH2P36)) {
				m_stream << (char) ((BCON_TOKEN_DATA36 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4)
						<< (char) ((len & 0xFF000) >> 12)
						<< (char) (len >> 20);
				m_stream << data;
			} else {
				throw SerializerException("Fatal: too big byte array (length=" + QString::number(len) + ")");
			}
			break;
		}
		default:
			throw SerializerException("Fatal: QVariant type not managed.");
		
	}
		
	if (key) m_stream << *key << '\0';
}

void Serializer::serializeJSON(const QVariant &variant) {
	switch (variant.type()) {
		case QVariant::Invalid:
		{
			m_stream << JSON_NULL;
			break;
		}
		case QVariant::Bool: // Case of JSON boolean
		{
			m_stream << (variant.toBool() ? JSON_TRUE: JSON_FALSE);
			break;
		}
		case QVariant::Map: // Case of JSON object
		{
			m_stream << JSON_OBJECT_BEGIN;
			const QVariantMap elements = variant.toMap();
			auto it = elements.begin();
			if (it != elements.end()) {
				m_stream << sanitizeString(it.key()) << JSON_MEMBER_SEP;
				serializeJSON(it.value());
				it++;
			}
			while (it != elements.end()) {
				m_stream << JSON_ELEMENT_SEP << sanitizeString(it.key()) << JSON_MEMBER_SEP;
				serializeJSON(it.value());
				it++;
			}
			m_stream << JSON_OBJECT_END;
			break;
		}
		case QVariant::List: // Case of JSON array
		{
			m_stream << JSON_ARRAY_BEGIN;
			const QVariantList elements = variant.toList();
			auto it = elements.begin();
			if (it != elements.end()) {
				serializeJSON(*it);
				it++;
			}
			while (it != elements.end()) {
				m_stream << JSON_ELEMENT_SEP;
				serializeJSON(*it);
				it++;
			}
			m_stream << JSON_ARRAY_END;
			break;
		}
		case QVariant::String:
		case QVariant::ByteArray: // Case of JSON string
		{
			m_stream << sanitizeString(variant.toString());
			break;
		}
		case QVariant::Char:
		case QVariant::Int:
		case QVariant::UInt:
		case QVariant::LongLong:
		case QVariant::ULongLong:
		{
			m_stream << QString::number(variant.toInt()).replace("inf", "infinity");
			break;
		}
		case QVariant::Double:
		{
			m_stream << QString::number(variant.toDouble()).replace("inf", "infinity");
			break;
		}
		default:
			throw SerializerException("Fatal: QVariant type not managed.");
	}
}

inline void Serializer::write32(QByteArray& output, uint32_t value) {
	output.append((char)((value & 0xFF)));
	output.append((char)(((value >> 8) & 0xFF)));
	output.append((char)(((value >> 16) & 0xFF)));
	output.append((char)(((value >> 24) & 0xFF)));
}

inline void Serializer::write64(QByteArray& output, uint64_t value) {
	output.append((char)((value & 0xFF)));
	output.append((char)(((value >> 8) & 0xFF)));
	output.append((char)(((value >> 16) & 0xFF)));
	output.append((char)(((value >> 24) & 0xFF)));
	output.append((char)(((value >> 32) & 0xFF)));
	output.append((char)(((value >> 40) & 0xFF)));
	output.append((char)(((value >> 48) & 0xFF)));
	output.append((char)(((value >> 56) & 0xFF)));
}

QByteArray Serializer::serializeBSONDocument(const QVariant &variant) {
	QByteArray payload;
	switch (variant.type()) {
		case QVariant::Map: // Case of BSON object
		{
			const QVariantMap elements = variant.toMap();
			for (auto it = elements.begin(); it != elements.end(); it++) {
				payload.append(serializeBSONElt(it.value(), it.key()));
			}
			break;
		}
		case QVariant::List: // Case of BSON array
		{
			const QVariantList elements = variant.toList();
			uint i = 0;
			for (auto it = elements.begin(); it != elements.end(); it++, i++) {
				payload.append(serializeBSONElt(*it, QString::number(i)));
			}
			break;
		}
		default:
			throw SerializerException("Fatal: Invalid document.");
	}
	QByteArray ret;
	write32(ret, payload.length() + 5);
	ret.append(payload);
	ret.append(BSON_TOKEN_END);
	return ret;
}

QByteArray Serializer::serializeBSONElt(const QVariant& variant, const QString &key) {
	QByteArray ret;
	switch (variant.type()) {
		case QVariant::Invalid:
			ret.append(BSON_TOKEN_NULL);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			break;
		case QVariant::Bool: // Case of BSON boolean
			ret.append(BSON_TOKEN_BOOL);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			ret.append(variant.toBool() ? BSON_TOKEN_TRUE: BSON_TOKEN_FALSE);
			break;
		case QVariant::UInt:
		case QVariant::Int:
		{
			ret.append(BSON_TOKEN_INT32);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write32(ret, variant.toUInt());
			break;
		}
		case QVariant::ULongLong:
		case QVariant::LongLong:
		{
			ret.append(BSON_TOKEN_INT64);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write64(ret, variant.toUInt());
			break;
		}
		case QVariant::Double:
		{
			ret.append(BSON_TOKEN_DOUBLE);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write64(ret, variant.toDouble());
			break;
		}
		case QVariant::DateTime:
		case QVariant::Date:
		case QVariant::Time:
		{
			ret.append(BSON_TOKEN_DATETIME);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write64(ret, variant.toDateTime().toMSecsSinceEpoch());
			break;
		}
		case QVariant::Map: // Case of BSON object
		{
			ret.append(BSON_TOKEN_MAP);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			ret.append(serializeBSONDocument(variant));
			break;
		}
		case QVariant::List: // Case of BSON array
		{
			ret.append(BSON_TOKEN_LIST);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			ret.append(serializeBSONDocument(variant));
			break;
		}
		case QVariant::String:
		{
			QByteArray data = variant.toString().toLocal8Bit();
			ret.append(BSON_TOKEN_STRING);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write32(ret, data.length() + 1);
			ret.append(data);
			ret.append('\0');
			break;
		}
		case QVariant::ByteArray: // Case of BSON string
		{
			QByteArray data = variant.toByteArray();
			ret.append(BSON_TOKEN_DATA);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write32(ret, data.length());
			ret.append(BSON_TOKEN_GENERIC);
			ret.append(data);
			break;
		}
		default:
			throw SerializerException("Fatal: QVariant type not managed.");
		
	}
	return ret;
}

}
