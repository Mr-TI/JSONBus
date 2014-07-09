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
#include "bsonserializer.h"
#include <qt4/QtCore/QVariant>

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
#define TBOOL		((char)0x08)
#define TMAP		((char)0x03)
#define TLIST		((char)0x04)
#define TGENERIC	((char)0x00)

namespace NodeBus {

/**
	* @brief ouput stream
	*/
class BSONChannelOutputStream :public BSONSerializer::OutputStream {
public:
	/**
	 * @brief ouput stream constructor from an std::ostream
	 * @param stream std::ostream reference
	 */
	BSONChannelOutputStream (const StreamChannelPtr& channel);
	
	/**
	 * @brief ouput stream destructor
	 */
	~BSONChannelOutputStream ();
	
	/**
	 * @brief output stream operator
	 */
	OutputStream& operator << (char byte);
	
	/**
	 * @brief output stream operator
	 */
	OutputStream& operator << (const QByteArray &data);
private:
	StreamChannelPtr m_channel;
};

inline BSONChannelOutputStream::BSONChannelOutputStream(const StreamChannelPtr& channel): m_channel(channel) {
}

inline BSONChannelOutputStream::~BSONChannelOutputStream() {
}

inline BSONSerializer::OutputStream& BSONChannelOutputStream::operator<<(char byte) {
	m_channel->write(&byte, 1);
	return *this; 
}

inline BSONSerializer::OutputStream& BSONChannelOutputStream::operator<<(const QByteArray& data) {
	m_channel->write(data.data(), data.length());
	return *this; 
}

/**
	* @brief Std ouput stream
	*/
class BSONByteArrayOutputStream :public BSONSerializer::OutputStream {
public:
	/**
		* @brief Std ouput stream constructor from an std::ostream
		* @param stream std::ostream reference
		*/
	BSONByteArrayOutputStream (QByteArray& data);
	
	/**
		* @brief Std ouput stream destructor
		*/
	~BSONByteArrayOutputStream();
	
	/**
	 * @brief output stream operator
	 */
	OutputStream& operator << (char byte);
	
	/**
	 * @brief output stream operator
	 */
	OutputStream& operator << (const QByteArray &data);
private:
	QByteArray& m_data;
};

BSONByteArrayOutputStream::BSONByteArrayOutputStream(QByteArray& data): m_data(data) {
}

BSONByteArrayOutputStream::~BSONByteArrayOutputStream() {
}

inline BSONSerializer::OutputStream& BSONByteArrayOutputStream::operator<<(char byte) {
	m_data.append(byte);
	return *this;
}

inline BSONSerializer::OutputStream& BSONByteArrayOutputStream::operator<<(const QByteArray& data) {
	m_data.append(data);
	return *this;
}

QString BSONSerializer::toString(const QVariant& variant) {
	QByteArray data;
	BSONSerializer(data).serialize(variant);
	return QString::fromLocal8Bit(data);
}

BSONSerializer::BSONSerializer(StreamChannelPtr channel)
: m_streamPtr(new BSONChannelOutputStream(channel)), m_stream(*m_streamPtr) {
	
}

BSONSerializer::BSONSerializer(QByteArray& data)
: m_streamPtr(new BSONByteArrayOutputStream(data)), m_stream(*m_streamPtr) {
}

BSONSerializer::BSONSerializer(BSONSerializer::OutputStream& stream)
: m_stream(stream) {
	
}

BSONSerializer::~BSONSerializer() {
}

inline void BSONSerializer::write32(QByteArray& output, uint32_t value) {
	output.append((char)((value & 0xFF)));
	output.append((char)(((value >> 8) & 0xFF)));
	output.append((char)(((value >> 16) & 0xFF)));
	output.append((char)(((value >> 24) & 0xFF)));
}

inline void BSONSerializer::write64(QByteArray& output, uint64_t value) {
	output.append((char)((value & 0xFF)));
	output.append((char)(((value >> 8) & 0xFF)));
	output.append((char)(((value >> 16) & 0xFF)));
	output.append((char)(((value >> 24) & 0xFF)));
	output.append((char)(((value >> 32) & 0xFF)));
	output.append((char)(((value >> 40) & 0xFF)));
	output.append((char)(((value >> 48) & 0xFF)));
	output.append((char)(((value >> 56) & 0xFF)));
}

void BSONSerializer::serialize(const QVariant &variant) {
	m_stream << serializeDocument(variant);
}

QByteArray BSONSerializer::serializeDocument(const QVariant &variant) {
	QByteArray payload;
	switch (variant.type()) {
		case QVariant::Map: // Case of BSON object
		{
			const QVariantMap elements = variant.toMap();
			for (auto it = elements.begin(); it != elements.end(); it++) {
				payload.append(serializeElt(it.value(), it.key()));
			}
			break;
		}
		case QVariant::List: // Case of BSON array
		{
			const QVariantList elements = variant.toList();
			uint i = 0;
			for (auto it = elements.begin(); it != elements.end(); it++, i++) {
				payload.append(serializeElt(*it, QString::number(i)));
			}
			break;
		}
		default:
			throw BSONSerializerException("Fatal: Invalid document.");
	}
	QByteArray ret;
	write32(ret, payload.length() + 1);
	ret.append(payload);
	ret.append(TEND);
	return ret;
}

QByteArray BSONSerializer::serializeElt(const QVariant& variant, const QString &key) {
	QByteArray ret;
	switch (variant.type()) {
		case QVariant::Invalid:
			ret.append(TNULL);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			break;
		case QVariant::Bool: // Case of BSON boolean
			ret.append(TBOOL);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			ret.append(variant.toBool() ? TTRUE: TFALSE);
			break;
		case QVariant::UInt:
		case QVariant::Int:
		{
			ret.append(TINT32);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write32(ret, variant.toUInt());
			break;
		}
		case QVariant::ULongLong:
		case QVariant::LongLong:
		{
			ret.append(TINT64);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write64(ret, variant.toUInt());
			break;
		}
		case QVariant::Double:
		{
			ret.append(TDOUBLE);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write64(ret, variant.toDouble());
			break;
		}
		case QVariant::DateTime:
		case QVariant::Date:
		case QVariant::Time:
		{
			ret.append(TDATETIME);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write64(ret, variant.toDateTime().toMSecsSinceEpoch());
			break;
		}
		case QVariant::Map: // Case of BSON object
		{
			ret.append(TMAP);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			ret.append(serializeDocument(variant));
			break;
		}
		case QVariant::List: // Case of BSON array
		{
			ret.append(TLIST);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			ret.append(serializeDocument(variant));
			break;
		}
		case QVariant::String:
		{
			QByteArray data = variant.toString().toLocal8Bit();
			ret.append(TSTRING);
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
			ret.append(TDATA);
			ret.append(key.toLocal8Bit());
			ret.append('\0');
			write32(ret, data.length());
			ret.append(TGENERIC);
			ret.append(data);
			break;
		}
		default:
			throw BSONSerializerException("Fatal: QVariant type not managed.");
		
	}
	return ret;
}

}
