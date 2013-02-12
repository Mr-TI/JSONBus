/*
 *   Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
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
 * @date 2013
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_JSONSERIALIZER_H
#define JSONBUS_JSONSERIALIZER_H

#include <jsonbus/core/exception.h>

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
class JSONBUS_EXPORT JSONSerializer: public QObject {
public:
	/**
	 * @brief Abstract ouput stream
	 */
	class Stream {
	public:
		/**
		 * @brief Abstract ouput stream constructor
		 */
		inline Stream () {};
		
		/**
		 * @brief Abstract ouput stream destructor
		 */
		inline virtual ~Stream () {};
		
		/**
		 * @brief Abstract output stream operator
		 * @param data Data to write
		 */
		virtual Stream& operator << (const QString &data) = 0;
		
		/**
		 * @brief Clone the output stream object
		 * @return a pointer to the coned StdStream object.
		 */
		virtual Stream* clone() const = 0;
	};
	
	/**
	 * @brief Std ouput stream
	 */
	class StdStream :public Stream {
	public:
		/**
		 * @brief Std ouput stream constructor from an std::ostream
		 * @param stream std::ostream reference
		 */
		inline StdStream (std::ostream &stream) :m_stream(stream) {};
		
		/**
		 * @brief Std ouput stream destructor
		 */
		inline virtual ~StdStream () {};
		
		/**
		 * @brief Std output stream operator
		 */
		inline virtual Stream& operator << (const QString &data) { m_stream << data.toUtf8().data(); return *this; };
		
		/**
		 * @brief Clone the output stream object
		 * @return a pointer to the coned StdStream object.
		 */
		inline virtual Stream* clone() const { return new StdStream(m_stream); };
	private:
		std::ostream &m_stream;
	};
	
	/**
	 * @brief JSONSerializer constructor.
	 * @param parent Parent object
	 */
	JSONSerializer(QObject* parent = 0);
	
	/**
	 * @brief JSONSerializer constructor.
	 * @param stream A reference to the std output stream
	 * @param parent Parent object
	 */
	JSONSerializer(std::ostream &stream, QObject* parent = 0);
	
	/**
	 * @brief JSONSerializer constructor.
	 * @param stream A reference to the output stream
	 * @param parent Parent object
	 */
	JSONSerializer(const Stream &stream, QObject* parent = 0);
	
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
	Stream &m_stream;
};

}

#endif
