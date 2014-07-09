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
 * @brief NodeBus : CSON serializer management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_CSONSERIALIZER_H
#define NODEBUS_CSONSERIALIZER_H

#include <nodebus/core/exception.h>
#include "streamchannel.h"

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#include <QString>
#include <QVariant>

namespace NodeBus {

nodebus_declare_exception(CSONSerializerException, Exception);

/**
 * @brief CSON serializer management.
 */
class NODEBUS_EXPORT BCONSerializer {
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
	 * @brief CSONSerializer constructor.
	 * @param stream A reference to the std output stream
	 */
	BCONSerializer(StreamChannelPtr channel);
	
	/**
	 * @brief CSONSerializer constructor.
	 * @param data Byte array reference
	 */
	BCONSerializer(QByteArray &data);
	
	/**
	 * @brief CSONSerializer constructor.
	 * @param stream A reference to the output stream
	 */
	BCONSerializer(OutputStream &stream);
	
	/**
	 * @brief CSONSerializer destructor.
	 */
	~BCONSerializer();
	
	/**
	 * @brief Serialize an object in CSON format
	 * @param variant object to serialize
	 */
	void serialize(const QVariant& variant, const char* key=NULL);
	
	/**
	 * @brief Serialize an object in CSON format
	 * @param variant object to serialize
	 */
	static QString toString(const QVariant &variant);
private:
	void write32(char type, uint32_t value);
	void write64(char type, uint64_t value);
	SharedPtr<OutputStream> m_streamPtr;
	OutputStream &m_stream;
};

inline BCONSerializer::OutputStream::OutputStream() {
}

inline BCONSerializer::OutputStream::~OutputStream() {
}

}

#endif
