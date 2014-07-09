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
 * @brief NodeBus : BSON Parser management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_BSONPARSER_H
#define NODEBUS_BSONPARSER_H

#include <nodebus/core/exception.h>
#include <qt4/QtCore/QByteArray>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

namespace NodeBus {

nodebus_declare_exception(BSONParserException, Exception);
nodebus_declare_exception(ErrorBSONParserException, BSONParserException);
class StreamChannel;

/**
 * @brief BSON Parser management.
 */
class NODEBUS_EXPORT BSONParser {
public:
	typedef char (*fGetc_t)(void *ptr);
	
	/**
	 * @brief BSONParser constructor.
	 */
	BSONParser();
	
	/**
	 * @brief BSONParser constructor.
	 * @param channel Channel pointer
	 */
	BSONParser(fGetc_t getChar, void *ptr);
	
	/**
	 * @brief BSONParser constructor.
	 * @param channel Channel pointer
	 */
	BSONParser(SharedPtr<StreamChannel> channel);
	
	/**
	 * @brief BSONParser destructor.
	 */
	~BSONParser();
	
	/**
	 * @brief Parse a BSON data from a byte array
	 * @return QVariant object
	 * @throw BSONParserException on parsing error
	 */
	QVariant parse();
	
	/**
	 * @brief Parse a BSON data from a byte array
	 * @return QVariant object
	 * @throw BSONParserException on parsing error
	 */
	static QVariant parse(const char *data, uint len);
	
	/**
	 * @brief Parse a BSON data from a byte array
	 * @return QVariant object
	 * @throw BSONParserException on parsing error
	 */
	static QVariant parse(const QByteArray &data);
	
private:
	QVariant parseDocument();
	bool parse(QVariant& res, QString& key);
	char getc();
	int32_t read32();
	int64_t read64();
	fGetc_t m_getChar;
	void *m_ptr;
};

}

#endif //NODEBUS_BSONPARSER_H
