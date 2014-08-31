/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
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

/**
 * @brief NodeBus : BSON serializer management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_SERIALIZER_H
#define NODEBUS_SERIALIZER_H

#include <nodebus/core/exception.h>
#include <nodebus/core/streamchannel.h>
#include <nodebus/core/global.h>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#include <QString>
#include <QVariant>

namespace NodeBus {

nodebus_declare_exception(SerializerException, Exception);

/**
 * @brief BSON serializer management.
 */
class NODEBUS_EXPORT Serializer {
public:
	/**
	 * @brief Abstract ouput stream
	 */
	class OutputStream: public SharedData {
	public:
		/**
		 * @brief Abstract ouput stream constructor
		 */
		OutputStream ();
		
		/**
		 * @brief Abstract ouput stream destructor
		 */
		virtual ~OutputStream ();
		
		/**
		 * @brief Abstract output stream operator
		 * @param byte Data to write
		 */
		virtual OutputStream& operator << (char byte) = 0;
		
		/**
		 * @brief Abstract output stream operator
		 * @param data Data to write
		 */
		virtual OutputStream& operator << (const QByteArray &data) = 0;
		
		/**
		 * @brief Abstract output stream operator
		 * @param data Data to write
		 */
		virtual OutputStream& operator << (const QString &data) = 0;
	};
	
	static uint32_t FORMAT_COMPACT;
	static uint8_t INDENT(uint8_t size);
	
	/**
	 * @brief Serializer constructor.
	 * @param stream A reference to the std output stream
	 */
	Serializer(StreamChannelPtr channel, FileFormat format=JSON);
	
	/**
	 * @brief Serializer constructor.
	 * @param data Byte array reference
	 */
	Serializer(QByteArray &data, FileFormat format=JSON);
	
	/**
	 * @brief Serializer constructor.
	 * @param stream A reference to the output stream
	 */
	Serializer(OutputStream &stream, FileFormat format=JSON);
	
	/**
	 * @brief Serializer destructor.
	 */
	~Serializer();
	
	/**
	 * @brief Serialize an object in BSON format
	 * @param variant object to serialize
	 */
	void serialize(const QVariant &variant, uint32_t flags = FORMAT_COMPACT);
	
	/**
	 * @brief Serialize an object in BSON format
	 * @param variant object to serialize
	 */
	static QString toJSONString(const QVariant &variant, uint32_t flags);
private:
	void serializeBCON(const QVariant &variant, const QString *key=NULL);
	void serializeJSON(const QVariant &variant, uint32_t flags);
	QByteArray serializeBSONDocument(const QVariant &variant);
	QByteArray serializeBSONElt(const QVariant& variant, const QString &key);
	void write16(char type, uint16_t value);
	void write32(char type, uint32_t value);
	void write64(char type, uint64_t value);
	void write32(QByteArray& output, uint32_t value);
	void write64(QByteArray& output, uint64_t value);
	SharedPtr<OutputStream> m_streamPtr;
	OutputStream &m_stream;
	FileFormat m_format;
};

inline uint8_t Serializer::INDENT(uint8_t size) {
	return size & 0x1Fu;
}

inline Serializer::OutputStream::OutputStream() {
}

inline Serializer::OutputStream::~OutputStream() {
}

}

#endif
