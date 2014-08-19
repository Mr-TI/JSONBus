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

#include "ltype.h"
#include <parser.hh>
#include "driver.h"

namespace idlparser {

#ifdef GNU_BISON_V2
void Parser::error(const Parser::location_type& l, const std::string& m) {
	driver.appendError(QString::fromStdString(m.c_str()));
}
#else
void Parser::error(const std::string& m) {
	driver.appendError(QString::fromStdString(m.c_str()));
}
#endif

}
