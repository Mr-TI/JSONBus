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

#include <nodebus/core/exception.h>

namespace NodeBus {

void __raise_InvalidClassException() {
	throw InvalidClassException();
}

void __raise_NullPointerException() {
	throw NullPointerException();
}

const char* Exception::what() const throw() {
	QString output = d->message;
	char **symTbl = backtrace_symbols(d->backtrace, d->backtraceSize);
	if (symTbl == NULL) {
		return output.toAscii().constData();
	}
	for (uint i = 0; i < d->backtraceSize; i++) {
		output.append('\n');
		output.append(symTbl[i]);
	}
	free(symTbl);
	return output.toLocal8Bit().constData();
}

}
