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
