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

/**
 * @brief NodeBus : BSON serializer management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_SERIALIZER_H
#define NODEBUS_SERIALIZER_H

#include <nodebus/core/exception.h>
#include "streamchannel.h"

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
	
	/// @brief Format
	enum Format {
		/// @brief JSON format
		JSON,
		/// @brief BCON format
		BCON,
		/// @brief BSON format
		BSON
	};
	
	/**
	 * @brief Serializer constructor.
	 * @param stream A reference to the std output stream
	 */
	Serializer(StreamChannelPtr channel, Format format=JSON);
	
	/**
	 * @brief Serializer constructor.
	 * @param data Byte array reference
	 */
	Serializer(QByteArray &data, Format format=JSON);
	
	/**
	 * @brief Serializer constructor.
	 * @param stream A reference to the output stream
	 */
	Serializer(OutputStream &stream, Format format=JSON);
	
	/**
	 * @brief Serializer destructor.
	 */
	~Serializer();
	
	/**
	 * @brief Serialize an object in BSON format
	 * @param variant object to serialize
	 */
	void serialize(const QVariant &variant);
	
	/**
	 * @brief Serialize an object in BSON format
	 * @param variant object to serialize
	 */
	static QString toJSONString(const QVariant &variant);
private:
	void serializeBCON(const QVariant &variant, const QString *key=NULL);
	void serializeJSON(const QVariant &variant);
	QByteArray serializeBSONDocument(const QVariant &variant);
	QByteArray serializeBSONElt(const QVariant& variant, const QString &key);
	void write32(char type, uint32_t value);
	void write64(char type, uint64_t value);
	void write32(QByteArray& output, uint32_t value);
	void write64(QByteArray& output, uint64_t value);
	SharedPtr<OutputStream> m_streamPtr;
	OutputStream &m_stream;
	Format m_format;
};

inline Serializer::OutputStream::OutputStream() {
}

inline Serializer::OutputStream::~OutputStream() {
}

}

#endif
