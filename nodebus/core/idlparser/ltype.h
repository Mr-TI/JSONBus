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

#ifndef IDLLTYPE_H
#define IDLLTYPE_H

#define YYSTYPE idlparser::LType

#define KNODE_TYPE         "T"
#define KNODE_DTYPE        "t"
#define KNODE_SNAME        "n"
#define KNODE_DIRECTION    "d"
#define KNODE_WRITABLE     "w"
#define KNODE_VALUE        "v"
#define KNODE_PARAMS       "p"
#define KNODE_PARENTS      "P"
#define KNODE_MEMBERS      "m"

#define VTYPE_VOID         "v"
#define VTYPE_ANY          "a"
#define VTYPE_BOOLEAN      "b"
#define VTYPE_BYTE         "o"
#define VTYPE_UINT32       "I"
#define VTYPE_INT32        "i"
#define VTYPE_UINT64       "L"
#define VTYPE_INT64        "l"
#define VTYPE_DOUBLE       "d" 
#define VTYPE_STRING       "s"
#define VTYPE_BYTEARRAY    "O"
#define VTYPE_DATETIME     "t"

#define NTYPE_DOCUMENT     "D"
#define NTYPE_CONST        "C"
#define NTYPE_ATTR         "A"
#define NTYPE_INTERFACE    "I"
#define NTYPE_METHOD       "M"
#define NTYPE_ENUM         "E"
#define NTYPE_STRUCT       "S"
#define NTYPE_TYPEDEF      "T"
#define NTYPE_PACKAGE      "P"

#define PDIR_IN            "i"
#define PDIR_OUT           "o"
#define PDIR_INOUT         "b"

#define NAMESPACE_SEP      "::"

#include <QVariant>
#include <QString>
#include <nodebus/core/shareddata.h>
#include <nodebus/core/sharedptr.h>
#include <nodebus/core/logger.h>
#include "driver.h"
#include "nodevariant.h"
#include "noderoot.h"

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

inline LType::LType(const LType& other): str(NULL) {
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

inline SharedPtr<NodeRoot> Driver::rootCtx() {
	return m_rootCtx;
}

inline NodePtr Driver::curCtx() {
	return m_curCtx;
}

inline NodePtr Driver::relolveConsVal(NodePtr pSyn) {
	NodePtr ret = m_curCtx->resolve(pSyn->str(), NTYPE_CONST);
	if (ret == nullptr) return new NodeVariant(0);
	return new NodeVariant(ret->map()[KNODE_VALUE]);
}

}

#endif // IDLLTYPE_H
