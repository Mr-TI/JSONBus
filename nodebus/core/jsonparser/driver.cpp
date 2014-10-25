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

#include <globals.h>
#include <parser.hh>
#include "scanner.h"
#include "driver.h"

#ifdef USE_NODEBUS_EXCEPTION
#include <nodebus/core/parser.h>
#define ErrorException ErrorParserException
using namespace NodeBus;
#else
#include "exception.h"
#endif


namespace jsonparser {

Driver::Driver(QDataStream &dataStream)
		: scanner(*new Scanner(dataStream)),
		parser(*new Parser(*this)){
}

Driver::~Driver() {
	delete &parser;
	delete &scanner;
}

QVariant Driver::parse() {
	variant_t ret;
	result = &ret;
	scanner.resetPos();
	if (parser.parse() != 0) {
		throw ErrorException(QString("Line: ") + QString::number(scanner.lineno()) + ", column: " + QString::number(scanner.YYLeng()) + ", " + lastError);
	}
	if (!result) {
		throw EOFException();
	}
	return *result;
}

}

