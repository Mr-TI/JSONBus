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

#ifndef IDLPARSER_DRIVER_H
#define IDLPARSER_DRIVER_H

#include <QVariant>
#include <QStack>
#include <QMap>
#include <stdio.h>

#define KNODE_TYPE         "T"
#define KNODE_DTYPE        "t"
#define KNODE_SNAME        "n"
#define KNODE_DIRECTION    "d"
#define KNODE_WRITABLE     "w"
#define KNODE_VALUE        "v"
#define KNODE_PARAMS       "p"

#define VTYPE_VOID         'v'
#define VTYPE_ANY          'a'
#define VTYPE_BOOLEAN      'b'
#define VTYPE_BYTE         'o'
#define VTYPE_UINT32       'I'
#define VTYPE_INT32        'i'
#define VTYPE_UINT64       'L'
#define VTYPE_INT64        'l'
#define VTYPE_DOUBLE       'd' 
#define VTYPE_STRING       's'
#define VTYPE_BYTEARRAY    'O'
#define VTYPE_DATETIME     't'

#define NTYPE_DOCUMENT     'D'
#define NTYPE_CONST        'C'
#define NTYPE_ATTR         'A'
#define NTYPE_INTERFACE    'I'
#define NTYPE_METHOD       'M'
#define NTYPE_ENUM         'E'
#define NTYPE_STRUCT       'S'
#define NTYPE_TYPEDEF      'T'
#define NTYPE_PACKAGE      'P'

#define PDIR_IN            'i'
#define PDIR_OUT           'o'
#define PDIR_INOUT         'b'

namespace idlparser {

class Parser;
class Scanner;

/**
 * @brief IDL parser driver
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class Driver {
public:
	typedef char (*getc_t)(void *);
	Driver(const QString &filename);
	~Driver();
	QVariant parse();
private:
	friend class Parser;
	friend class Scanner;
	QString lastError;
	FILE *m_stream;
	Scanner &scanner;
	Parser &parser;
	QVariant result;
};

}

#endif // IDLPARSER_DRIVER_H
