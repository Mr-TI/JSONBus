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
}

namespace JSONBus {

jsonbus_declare_exception(JSONParserException, Exception);
jsonbus_declare_exception(EOFJSONParserException, JSONParserException);
jsonbus_declare_exception(ErrorJSONParserException, JSONParserException);
class StreamChannel;

/**
 * @brief JSON Parser management.
 */
class JSONBUS_EXPORT JSONParser : public SharedData {
public:
	
	/**
	 * @brief JSONParser constructor.
	 */
	JSONParser();
	
	/**
	 * @brief JSONParser constructor.
	 * @param channel Channel pointer
	 */
	JSONParser(const SharedPtr<StreamChannel> &channel);
	
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
	
	void cancel();
	
private:
	static char s_getc(void *stream);
	
	SharedPtr<StreamChannel> m_channel;
	jsonparser::Driver *m_driver;
};

}

#endif //JSONBUS_JSONPARSER_H
