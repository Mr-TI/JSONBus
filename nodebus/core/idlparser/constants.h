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

#ifndef IDLPARSER_CONSTANTS_H
#define IDLPARSER_CONSTANTS_H

namespace idlparser {

const char * const NODE_KEY_VERSION    = "V";
const char * const NODE_KEY_NODE_TYPE  = "T";
const char * const NODE_KEY_DATA_TYPE  = "t";
const char * const NODE_KEY_NAME       = "n";
const char * const NODE_KEY_DIRECTION  = "d";
const char * const NODE_KEY_WRITABLE   = "w";
const char * const NODE_KEY_VALUE      = "v";
const char * const NODE_KEY_PARAMS     = "p";
const char * const NODE_KEY_PARENTS    = "P";
const char * const NODE_KEY_MEMBERS    = "m";

const char * const TYPE_VAL_VOID       = "v";
const char * const TYPE_VAL_ANY        = "a";
const char * const TYPE_VAL_BOOLEAN    = "b";
const char * const TYPE_VAL_BYTE       = "o";
const char * const TYPE_VAL_UINT16     = "S";
const char * const TYPE_VAL_INT16      = "s";
const char * const TYPE_VAL_UINT32     = "I";
const char * const TYPE_VAL_INT32      = "i";
const char * const TYPE_VAL_UINT64     = "L";
const char * const TYPE_VAL_INT64      = "l";
const char * const TYPE_VAL_DOUBLE     = "d"; 
const char * const TYPE_VAL_STRING     = "c";
const char * const TYPE_VAL_BYTEARRAY  = "O";
const char * const TYPE_VAL_DATETIME   = "t";

const char * const TYPE_NODE_FRAGMENT  = "F";
const char * const TYPE_NODE_BASE      = "B";
const char * const TYPE_NODE_CONST     = "C";
const char * const TYPE_NODE_ATTR      = "A";
const char * const TYPE_NODE_INTERFACE = "I";
const char * const TYPE_NODE_METHOD    = "M";
const char * const TYPE_NODE_ENUM      = "E";
const char * const TYPE_NODE_STRUCT    = "S";
const char * const TYPE_NODE_TYPEDEF   = "T";
const char * const TYPE_NODE_PACKAGE   = "P";

const char * const PARAM_DIR_IN        = "i";
const char * const PARAM_DIR_OUT       = "o";
const char * const PARAM_DIR_INOUT     = "b";

const char * const NAMESPACE_SEP       = "::";

}

#endif // IDLPARSER_CONSTANTS_H
