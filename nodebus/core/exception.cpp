/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
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
