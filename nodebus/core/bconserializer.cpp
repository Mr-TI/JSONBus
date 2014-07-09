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
#include "bconserializer.h"
#include <qt4/QtCore/QVariant>

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
#define TDATETIME	((char)0x0B)
#define TMAP		((char)0x0C)
#define TLIST		((char)0x0D)
#define TDATA6		((char)0xA0)
#define TSTRING6	((char)0xC0)
#define TDATA12		((char)0x10)
#define TDATA20		((char)0x20)
#define TDATA36		((char)0x30)
#define TSTRING12	((char)0x50)
#define TSTRING20	((char)0x60)
#define TSTRING36	((char)0x70)

#define LENGTH2P6	64
#define LENGTH2P12	4096
#define LENGTH2P20	1048576
#define LENGTH2P36	68719476736

namespace NodeBus {

/**
	* @brief ouput stream
	*/
class BCONChannelOutputStream :public BCONSerializer::OutputStream {
public:
	/**
	 * @brief ouput stream constructor from an std::ostream
	 * @param stream std::ostream reference
	 */
	BCONChannelOutputStream (const StreamChannelPtr& channel);
	
	/**
	 * @brief ouput stream destructor
	 */
	~BCONChannelOutputStream ();
	
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

inline BCONChannelOutputStream::BCONChannelOutputStream(const StreamChannelPtr& channel): m_channel(channel) {
}

inline BCONChannelOutputStream::~BCONChannelOutputStream() {
}

inline BCONSerializer::OutputStream& BCONChannelOutputStream::operator<<(char byte) {
	m_channel->write(&byte, 1);
	return *this; 
}

inline BCONSerializer::OutputStream& BCONChannelOutputStream::operator<<(const QByteArray& data) {
	m_channel->write(data.data(), data.length());
	return *this; 
}

/**
	* @brief Std ouput stream
	*/
class BCONByteArrayOutputStream :public BCONSerializer::OutputStream {
public:
	/**
		* @brief Std ouput stream constructor from an std::ostream
		* @param stream std::ostream reference
		*/
	BCONByteArrayOutputStream (QByteArray& data);
	
	/**
		* @brief Std ouput stream destructor
		*/
	~BCONByteArrayOutputStream();
	
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

BCONByteArrayOutputStream::BCONByteArrayOutputStream(QByteArray& data): m_data(data) {
}

BCONByteArrayOutputStream::~BCONByteArrayOutputStream() {
}

inline BCONSerializer::OutputStream& BCONByteArrayOutputStream::operator<<(char byte) {
	m_data.append(byte);
	return *this;
}

inline BCONSerializer::OutputStream& BCONByteArrayOutputStream::operator<<(const QByteArray& data) {
	m_data.append(data);
	return *this;
}

QString BCONSerializer::toString(const QVariant& variant) {
	QByteArray data;
	BCONSerializer(data).serialize(variant);
	return QString::fromLocal8Bit(data);
}

BCONSerializer::BCONSerializer(StreamChannelPtr channel)
: m_streamPtr(new BCONChannelOutputStream(channel)), m_stream(*m_streamPtr) {
	
}

BCONSerializer::BCONSerializer(QByteArray& data)
: m_streamPtr(new BCONByteArrayOutputStream(data)), m_stream(*m_streamPtr) {
}

BCONSerializer::BCONSerializer(BCONSerializer::OutputStream& stream)
: m_stream(stream) {
	
}

BCONSerializer::~BCONSerializer() {
}

inline void BCONSerializer::write32(char type, uint32_t value) {
	m_stream << type
		<< ((char)((value & 0xFF)))
		<< ((char)(((value >> 8) & 0xFF)))
		<< ((char)(((value >> 16) & 0xFF)))
		<< ((char)(((value >> 24) & 0xFF)));
}

inline void BCONSerializer::write64(char type, uint64_t value) {
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

void BCONSerializer::serialize(const QVariant &variant, const char *key) {
	switch (variant.type()) {
		case QVariant::Invalid:
			m_stream << TNULL;
			break;
		case QVariant::Bool: // Case of BCON boolean
			m_stream << (variant.toBool() ? TTRUE: TFALSE);
			break;
		case QVariant::Char: // Case of BCON boolean
			m_stream << TBYTE
					<< variant.toChar().toAscii();
			break;
		case QVariant::UInt: {
			write32(TUINT32, variant.toUInt());
			break;
		}
		case QVariant::ULongLong:
		{
			write64(TUINT64, variant.toUInt());
			break;
		}
		case QVariant::Int:
		{
			write32(TINT32, variant.toUInt());
			break;
		}
		case QVariant::LongLong:
		{
			write64(TINT64, variant.toUInt());
			break;
		}
		case QVariant::Double:
		{
			write64(TDOUBLE, variant.toDouble());
			break;
		}
		case QVariant::DateTime:
		case QVariant::Date:
		case QVariant::Time:
		{
			write64(TDATETIME, variant.toDateTime().toMSecsSinceEpoch());
			break;
		}
		case QVariant::Map: // Case of BCON object
		{
			m_stream << TMAP;
			const QVariantMap elements = variant.toMap();
			for (auto it = elements.begin(); it != elements.end(); it++) {
				serialize(it.value(), it.key().toLocal8Bit().constData());
			}
			m_stream << TEND;
			break;
		}
		case QVariant::List: // Case of BCON array
		{
			m_stream << TLIST;
			const QVariantList elements = variant.toList();
			for (auto it = elements.begin(); it != elements.end(); it++) {
				serialize(*it);
			}
			m_stream << TEND;
			break;
		}
		case QVariant::String:
		{
			QByteArray data = variant.toString().toLocal8Bit();
			int len = data.length();
			if (len < (LENGTH2P6)) {
				m_stream << (char) ((TSTRING6 & 0xFF) | (len & 0x3F));
				m_stream << data;
			} else if (len < (LENGTH2P12)) {
				m_stream << (char) ((TSTRING12 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4);
				m_stream << data;
			} else if (len < (LENGTH2P20)) {
				m_stream << (char) ((TSTRING20 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4)
						<< (char) ((len & 0xFF000) >> 12);
				m_stream << data;
			} else if (len < (LENGTH2P36)) {
				m_stream << (char) ((TSTRING20 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4)
						<< (char) ((len & 0xFF000) >> 12)
						<< (char) (len >> 20);
				m_stream << data;
			} else {
				throw BCONSerializerException("Fatal: too big String (length=" + QString::number(len) + ")");
			}
			break;
		}
		case QVariant::ByteArray: // Case of BCON string
		{
			QByteArray data = variant.toByteArray();
			int len = data.length();
			if (len < (LENGTH2P6)) {
				m_stream << (char) ((TDATA6 & 0xFF) | (len & 0x3F));
				m_stream << data;
			} else if (len < (LENGTH2P12)) {
				m_stream << (char) ((TDATA12 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4);
				m_stream << data;
			} else if (len < (LENGTH2P20)) {
				m_stream << (char) ((TDATA20 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4)
						<< (char) ((len & 0xFF000) >> 12);
				m_stream << data;
			} else if (len < (LENGTH2P36)) {
				m_stream << (char) ((TDATA20 & 0xFF) | (len & 0x0F))
						<< (char) ((len & 0xFF0) >> 4)
						<< (char) ((len & 0xFF000) >> 12)
						<< (char) (len >> 20);
				m_stream << data;
			} else {
				throw BCONSerializerException("Fatal: too big byte array (length=" + QString::number(len) + ")");
			}
			break;
		}
		default:
			throw BCONSerializerException("Fatal: QVariant type not managed.");
		
	}
		
	if (key) m_stream << key << '\0';
}

}