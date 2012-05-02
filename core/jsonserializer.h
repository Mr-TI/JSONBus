/*
    Copyright (c) 2012, Emeric Verschuur <contact@openihs.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the OpenIHS.org nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @brief JSONBus : JSON serializer management.
 * @file jsonserializer.h
 * @author Emeric VERSCHUUR <contact@openihs.org>, (C) 2012
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
