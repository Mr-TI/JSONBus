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
