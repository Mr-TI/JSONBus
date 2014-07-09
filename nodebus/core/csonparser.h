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
 * @brief NodeBus : CSON Parser management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_CSONPARSER_H
#define NODEBUS_CSONPARSER_H

#include <nodebus/core/exception.h>
#include <qt4/QtCore/QByteArray>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

namespace NodeBus {

nodebus_declare_exception(CSONParserException, Exception);
nodebus_declare_exception(ErrorCSONParserException, CSONParserException);
class StreamChannel;

/**
 * @brief CSON Parser management.
 */
class NODEBUS_EXPORT CSONParser {
public:
	typedef char (*fGetc_t)(void *ptr);
	
	/**
	 * @brief CSONParser constructor.
	 */
	CSONParser();
	
	/**
	 * @brief CSONParser constructor.
	 * @param channel Channel pointer
	 */
	CSONParser(fGetc_t getChar, void *ptr);
	
	/**
	 * @brief CSONParser constructor.
	 * @param channel Channel pointer
	 */
	CSONParser(SharedPtr<StreamChannel> channel);
	
	/**
	 * @brief CSONParser destructor.
	 */
	~CSONParser();
	
	/**
	 * @brief Parse a CSON data from a byte array
	 * @return QVariant object
	 * @throw CSONParserException on parsing error
	 */
	QVariant parse();
	
	/**
	 * @brief Parse a CSON data from a byte array
	 * @return QVariant object
	 * @throw CSONParserException on parsing error
	 */
	static QVariant parse(const char *data, uint len);
	
	/**
	 * @brief Parse a CSON data from a byte array
	 * @return QVariant object
	 * @throw CSONParserException on parsing error
	 */
	static QVariant parse(const QByteArray &data);
	
private:
	bool parse(QVariant &res, QString* key);
	char getc();
	uint32_t read32();
	uint64_t read64();
	fGetc_t m_getChar;
	void *m_ptr;
};

}

#endif //NODEBUS_CSONPARSER_H
