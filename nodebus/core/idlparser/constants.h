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

#ifndef IDLPARSER_CONSTANTS_H
#define IDLPARSER_CONSTANTS_H

namespace idlparser {

const char * const NODE_KEY_VERSION    = "V";
const char * const NODE_KEY_TYPE       = "T";
const char * const NODE_KEY_DTYPE      = "t";
const char * const NODE_KEY_SNAME      = "n";
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
const char * const TYPE_VAL_UINT32     = "I";
const char * const TYPE_VAL_INT32      = "i";
const char * const TYPE_VAL_UINT64     = "L";
const char * const TYPE_VAL_INT64      = "l";
const char * const TYPE_VAL_DOUBLE     = "d"; 
const char * const TYPE_VAL_STRING     = "s";
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
