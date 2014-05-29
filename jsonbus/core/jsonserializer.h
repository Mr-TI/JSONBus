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
 * @brief JSONBus : JSON serializer management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_JSONSERIALIZER_H
#define JSONBUS_JSONSERIALIZER_H

#include <jsonbus/core/exception.h>
#include "streamchannel.h"

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#include <QString>
#include <QVariant>

namespace JSONBus {

jsonbus_declare_exception(JSONSerializerException, Exception);

/**
 * @brief JSON serializer management.
 */
class JSONBUS_EXPORT JSONSerializer {
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
		virtual OutputStream& operator << (const QString &data) = 0;
	};
	
	/**
	 * @brief JSONSerializer constructor.
	 * @param stream A reference to the std output stream
	 */
	JSONSerializer(StreamChannelPtr channel);
	
	/**
	 * @brief JSONSerializer constructor.
	 * @param data Byte array reference
	 */
	JSONSerializer(QByteArray &data);
	
	/**
	 * @brief JSONSerializer constructor.
	 * @param stream A reference to the output stream
	 */
	JSONSerializer(OutputStream &stream);
	
	/**
	 * @brief JSONSerializer destructor.
	 */
	~JSONSerializer();
	
	/**
	 * @brief Serialize an object in JSON format
	 * @param variant object to serialize
	 */
	void serialize(const QVariant &variant);
private:
	SharedPtr<OutputStream> m_streamPtr;
	OutputStream &m_stream;
};

inline JSONSerializer::OutputStream::OutputStream() {
}

inline JSONSerializer::OutputStream::~OutputStream() {
}

}

#endif
