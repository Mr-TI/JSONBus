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
 * @brief JSONBus : JSON Parser management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
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
class AbstractStreamBuf;
}

namespace JSONBus {

jsonbus_declare_exception(JSONParserException, Exception);
jsonbus_declare_exception(EOFJSONParserException, JSONParserException);
jsonbus_declare_exception(ErrorJSONParserException, JSONParserException);

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
	 * @param fd File descriptor
	 * @param parent Parent object
	 */
	JSONParser(int fd, QObject* parent = 0);
	
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
	 * @param stream File descriptor
	 * @return QVariant object
	 * @throw JSONParserException on parsing error
	 */
	QVariant parse(int df);
	
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
	
	void disable();
	
private:
	void setupBuf(QIODevice &device);
	void setupBuf(QTextStream& stream);
	void setupBuf(std::istream &stream);
	void setupBuf(int fd);
	void cleanup();
	jsonparser::Driver *m_driver;
	std::istream *m_inputStream;
	jsonparser::AbstractStreamBuf *m_buffer;
};

}

#endif //JSONBUS_JSONPARSER_H
