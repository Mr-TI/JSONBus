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

#ifndef NODEBUS_BSONSERIALIZER_H
#define NODEBUS_BSONSERIALIZER_H

#include <nodebus/core/exception.h>
#include "streamchannel.h"

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#include <QString>
#include <QVariant>

namespace NodeBus {

nodebus_declare_exception(BSONSerializerException, Exception);

/**
 * @brief BSON serializer management.
 */
class NODEBUS_EXPORT BSONSerializer {
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
		 * @param data Data to write
		 */
		virtual OutputStream& operator << (char byte) = 0;
		
		/**
		 * @brief Abstract output stream operator
		 * @param data Data to write
		 */
		virtual OutputStream& operator << (const QByteArray &data) = 0;
	};
	
	/**
	 * @brief BSONSerializer constructor.
	 * @param stream A reference to the std output stream
	 */
	BSONSerializer(StreamChannelPtr channel);
	
	/**
	 * @brief BSONSerializer constructor.
	 * @param data Byte array reference
	 */
	BSONSerializer(QByteArray &data);
	
	/**
	 * @brief BSONSerializer constructor.
	 * @param stream A reference to the output stream
	 */
	BSONSerializer(OutputStream &stream);
	
	/**
	 * @brief BSONSerializer destructor.
	 */
	~BSONSerializer();
	
	/**
	 * @brief Serialize an object in BSON format
	 * @param variant object to serialize
	 */
	void serialize(const QVariant &variant);
	
	/**
	 * @brief Serialize an object in BSON format
	 * @param variant object to serialize
	 */
	static QString toString(const QVariant &variant);
private:
	QByteArray serializeDocument(const QVariant &variant);
	QByteArray serializeElt(const QVariant& variant, const QString &key);
	void write32(QByteArray& output, uint32_t value);
	void write64(QByteArray& output, uint64_t value);
	SharedPtr<OutputStream> m_streamPtr;
	OutputStream &m_stream;
};

inline BSONSerializer::OutputStream::OutputStream() {
}

inline BSONSerializer::OutputStream::~OutputStream() {
}

}

#endif
