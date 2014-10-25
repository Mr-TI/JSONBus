/*
 * Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * @brief NodeBus : BCON Parser management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_PARSER_H
#define NODEBUS_PARSER_H

#include <nodebus/core/exception.h>
#include <nodebus/core/global.h>
#include <QByteArray>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

namespace NodeBus {

nodebus_declare_exception(ParserException, Exception);
nodebus_declare_exception(ErrorParserException, ParserException);

/**
 * @brief BCON Parser management.
 */
class NODEBUS_EXPORT Parser {
public:
	/**
	 * @brief Parser constructor.
	 * @param channel Channel pointer
	 */
	Parser(QDataStream &dataStream, FileFormat format = JSON);
	
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
	static QVariant fromFile(const QString &fileName, FileFormat format = JSON);
	
	/**
	 * @brief Parse a BCON data from a byte array
	 * @return QVariant object
	 * @throw ParserException on parsing error
	 */
	static QVariant parse(const char *data, uint len, FileFormat format = JSON);
	
	/**
	 * @brief Parse a BCON data from a byte array
	 * @return QVariant object
	 * @throw ParserException on parsing error
	 */
	static QVariant parse(const QByteArray &data, FileFormat format = JSON);
	
private:
	bool parseBCON(QVariant &res, QString* key);
	template <typename T> T read();
	FileFormat m_format;
	QDataStream &m_dataStream;
	QVariant parseBSONDocument();
	bool parseBSONElt(QVariant &res, QString &key);
	void *m_driver;
};

}

#endif //NODEBUS_PARSER_H
