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

#ifndef IDLPARSER_NODEMODULE_H
#define IDLPARSER_NODEMODULE_H

#include "ltype.h"
#include "noderoot.h"
#include "nodevariant.h"

namespace idlparser {

class NodeModule: public Node {
protected :
	Driver &m_driver;
public:
	NodeModule(Driver &driver, NodePtr &pSym);
	virtual const QVariant &resolve(const QString symbol, const char *type);
	virtual bool append(NodePtr &pElt);
	virtual QString str();
	QString m_prefix;
	QVariantMap m_symTbl;
};

inline NodeModule::NodeModule(Driver &driver, NodePtr &pSym)
: m_driver(driver), m_prefix(driver.curCtx()->str() + pSym->str() + "::") {

}

inline bool NodeModule::append(NodePtr &pElt) {
	return m_driver.rootCtx()->append(pElt);
}

inline QString NodeModule::str() {
	return m_prefix;
}

inline const QVariant &NodeModule::resolve(const QString symbol, const char *type) {
	static const QVariant zero;
	QVariant result;
	if (m_symTbl.contains(symbol)) {
		result = m_symTbl[symbol];
	} else if (m_driver.rootCtx()->m_symTbl.contains(symbol)) {
		result = m_driver.rootCtx()->m_symTbl[symbol];
	} else {
		m_driver.appendError("Undefined symbol " + symbol);
		return zero;
	}
	QVariantMap mNode = result.toMap();
	if (mNode[KNODE_TYPE].toString().at(0) == type[0]) {
		return mNode[KNODE_VALUE];
	} else {
		m_driver.appendError("Incompatible symbol " + symbol);
		return zero;
	}
}

}

#endif // IDLPARSER_NODEMODULE_H
