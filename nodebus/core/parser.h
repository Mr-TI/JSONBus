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
 * @brief NodeBus : BCON Parser management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_PARSER_H
#define NODEBUS_PARSER_H

#include <nodebus/core/exception.h>
#include <nodebus/core/global.h>
#include <qt4/QtCore/QByteArray>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

namespace NodeBus {

nodebus_declare_exception(ParserException, Exception);
nodebus_declare_exception(ErrorParserException, ParserException);
class StreamChannel;

/**
 * @brief BCON Parser management.
 */
class NODEBUS_EXPORT Parser {
public:
	typedef char (*fGetc_t)(void *ptr);
	
	/**
	 * @brief Parser constructor.
	 * @param channel Channel pointer
	 */
	Parser(fGetc_t getChar, void *ptr, DataFormat format = JSON);
	
	/**
	 * @brief Parser constructor.
	 * @param channel Channel pointer
	 */
	Parser(SharedPtr<StreamChannel> channel, DataFormat format = JSON);
	
	/**
	 * @brief Parser destructor.
	 */
	~Parser();
	
	/**
	 * @brief Parse a BCON data from a byte array
	 * @return QVariant object
	 * @throw ParserException on parsing error
	 */
	QVariant parse();
	
	/**
	 * @brief Parse a BCON data from a byte array
	 * @return QVariant object
	 * @throw ParserException on parsing error
	 */
	static QVariant parse(const char *data, uint len, DataFormat format = JSON);
	
	/**
	 * @brief Parse a BCON data from a byte array
	 * @return QVariant object
	 * @throw ParserException on parsing error
	 */
	static QVariant parse(const QByteArray &data, DataFormat format = JSON);
	
private:
	bool parseBCON(QVariant &res, QString* key);
	char getc();
	uint32_t read32();
	uint64_t read64();
	DataFormat m_format;
	SharedPtr<StreamChannel> m_channel;
	fGetc_t m_getChar;
	void *m_ptr;
	QVariant parseBSONDocument();
	bool parseBSONElt(QVariant &res, QString &key);
};

}

#endif //NODEBUS_PARSER_H
