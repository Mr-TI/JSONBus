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
 * @brief JSONBus : JSON Parser management.
 * @file jsonparser.h
 * @author Emeric VERSCHUUR <contact@openihs.org>, (C) 2012
 */

#ifndef JSONBUS_JSONPARSER_H
#define JSONBUS_JSONPARSER_H

#include <jsonbus/core/exception.h>

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#include <QString>
#include <QTextStream>
#include <QVariant>

namespace jsonparser {
class Driver;
class IODeviceBuf;
class TextStreamBuf;
}

namespace JSONBus {

jsonbus_declare_exception(JSONParserException, Exception);
jsonbus_declare_exception(JSONParserEOFException, JSONParserException);
jsonbus_declare_exception(JSONParserErrorException, JSONParserException);

/**
 * @brief JSON Parser management.
 */
class JSONBUS_EXPORT JSONParser: public QObject {
public:
	
	/**
	 * @brief JSONParser constructor.
	 * @param parent Parent object
	 */
	JSONParser(QObject* parent = 0);
	
	/**
	 * @brief JSONParser constructor.
	 * @param stream STD input stream to get data from
	 * @param parent Parent object
	 */
	JSONParser(std::istream &stream, QObject* parent = 0);
	
	/**
	 * @brief JSONParser constructor.
	 * @param stream Text stream to get data from
	 * @param parent Parent object
	 */
	JSONParser(QTextStream &stream, QObject* parent = 0);
	
	/**
	 * @brief JSONParser constructor.
	 * @param input Device to get data from
	 * @param parent Parent object
	 */
	JSONParser(QIODevice &input, QObject* parent = 0);
	
	/**
	 * @brief JSONParser destructor.
	 */
	~JSONParser();
	
	/**
	 * @brief Parse a JSON data from a byte array
	 * @return QVariant object
	 * @throw JSONParserException on parsing error
	 */
	QVariant parse();
	
	/**
	 * @brief Parse a JSON data from a byte array
	 * @param data data to parse
	 * @return QVariant object
	 * @throw JSONParserException on parsing error
	 */
	QVariant parse(const QByteArray& data);
	
	/**
	 * @brief Parse a JSON data from a input device
	 * @param stream STD input stream to get data from
	 * @return QVariant object
	 * @throw JSONParserException on parsing error
	 */
	QVariant parse(std::istream &stream);
	
	/**
	 * @brief Parse a JSON data from a input device
	 * @param stream Text stream to get data from
	 * @return QVariant object
	 * @throw JSONParserException on parsing error
	 */
	QVariant parse(QTextStream &stream);
	
	/**
	 * @brief Parse a JSON data from a input device
	 * @param input Device to get data from
	 * @return QVariant object
	 * @throw JSONParserException on parsing error
	 */
	QVariant parse(QIODevice& input);
	
private:
	void setupIODeviceBuf(QIODevice &device);
	void setupTextStreamBuf(QTextStream &device);
	void cleanup();
	jsonparser::Driver *m_driver;
	std::istream *m_inputStream;
	jsonparser::IODeviceBuf *m_devBuf;
	jsonparser::TextStreamBuf *m_tsBuf;
};

}

#endif //JSONBUS_JSONPARSER_H
