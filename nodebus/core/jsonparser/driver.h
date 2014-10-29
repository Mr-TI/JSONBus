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

#ifndef JSONPARSER_DRIVER_H
#define JSONPARSER_DRIVER_H

#include <nodebus/core/datastream.h>
#include <QVariant>

namespace jsonparser {

class Parser;
class Scanner;
class r;

/**
 * @brief JSON parser driver
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */
class Driver {
public:
	typedef char (*getc_t)(void *);
	Driver(NodeBus::DataStream &dataStream);
	~Driver();
	QVariant parse();
private:
	QString lastError;
	Scanner &scanner;
	Parser &parser;
	QVariant *result;
	friend class Parser;
	friend class Scanner;
	friend class r;
};

}

#endif // JSONPARSER_DRIVER_H
