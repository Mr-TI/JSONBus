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

#ifndef IDLLTYPE_H
#define IDLLTYPE_H

#define YYSTYPE idlparser::LType

#include <QVariant>
#include <QString>
#include <nodebus/core/shareddata.h>
#include <nodebus/core/sharedptr.h>
#include <nodebus/core/logger.h>
#include "driver.h"
#include "nodevariant.h"

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

using namespace NodeBus;

namespace idlparser {
	
class Driver;

class LType: public SharedData {
public:
	LType();
	LType(const LType& other);
	virtual ~LType();
	NodePtr node;
	QString *str;
	LType& operator= (const LType& other);
};

inline LType::LType(): str(NULL) {
}

inline LType::LType(const LType& other) {
	node = other.node;
	if (other.str) str = new QString(*(other.str));
}

inline LType::~LType() {
	delete str;
}

inline LType& LType::operator=(const LType& other) {
	node = other.node;
	if (other.str) str = new QString(*(other.str));
	return *this;
}

inline bool opexec(NodePtr &res, char op, NodePtr &op1_n, NodePtr &op2_n, Driver &driver) {
	QVariant &op1 = op1_n->val();
	QVariant &op2 = op2_n->val();
	if (!op1.canConvert(QVariant::Double)) {
		return driver.appendError("Error: invalid operand " + Logger::dump(op1));
	} else if (!op2.canConvert(QVariant::Double)) {
		return driver.appendError("Error: invalid operand " + Logger::dump(op2));
	} else if (op1.type() == QVariant::Double || op2.type() == QVariant::Double) {
		switch (op) {
			case '+':
				res = new NodeVariant(op1.toDouble() + op1.toDouble());
				break;
			case '-':
				res = new NodeVariant(op1.toDouble() + op1.toDouble());
				break;
			case '*':
				res = new NodeVariant(op1.toDouble() + op1.toDouble());
				break;
			case '/':
				res = new NodeVariant(op1.toDouble() + op1.toDouble());
				break;
			case '%':
				res = new NodeVariant(op1.toLongLong() % op1.toLongLong());
				break;
			default:
				return driver.appendError("Error: invalid operator " + char(op));
		}
		
	} else {
		switch (op) {
			case '+':
				res = new NodeVariant(op1.toLongLong() + op1.toLongLong());
				break;
			case '-':
				res = new NodeVariant(op1.toLongLong() + op1.toLongLong());
				break;
			case '*':
				res = new NodeVariant(op1.toLongLong() + op1.toLongLong());
				break;
			case '/':
				res = new NodeVariant(op1.toLongLong() + op1.toLongLong());
				break;
			case '%':
				res = new NodeVariant(op1.toLongLong() % op1.toLongLong());
				break;
			default:
				return driver.appendError("Error: invalid operator " + char(op));
		}
	}
	return true;
}

inline NodePtr Driver::shared() {
	return m_shared;
}

}

#endif // IDLLTYPE_H
