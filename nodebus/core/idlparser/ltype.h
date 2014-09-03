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

#include "constants.h"
#ifndef IDLPARSER_LTYPE_H
#define IDLPARSER_LTYPE_H

#define YYSTYPE idlparser::LType

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
	NodePtr ret = m_curCtx->resolve(pSyn->str(), TYPE_NODE_CONST);
	if (ret == nullptr) return new NodeVariant(0);
	return new NodeVariant(ret->map()[NODE_KEY_VALUE]);
}

}

#endif // IDLPARSER_LTYPE_H
