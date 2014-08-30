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

#ifndef IDLPARSER_NODEMODULE_H
#define IDLPARSER_NODEMODULE_H

#include "ltype.h"
#include "noderoot.h"
#include "nodevariant.h"
#include "nodemap.h"

namespace idlparser {

class NodeModule: public Node {
protected :
	Driver &m_driver;
public:
	NodeModule(Driver &driver, NodePtr &pSym);
	virtual NodePtr resolve(const QString symbol, const char *type);
	virtual bool append(NodePtr &pElt);
	virtual QString str();
	QString m_prefix;
};

inline NodeModule::NodeModule(Driver &driver, NodePtr &pSym)
: m_driver(driver), m_prefix(driver.curCtx()->str() + pSym->str() + NAMESPACE_SEP) {

}

inline bool NodeModule::append(NodePtr &pElt) {
	QVariantMap elt = pElt->map();
	QString name = elt[KNODE_SNAME].toString();
	if (m_driver.rootCtx()->m_symTbl.contains(name)) {
		m_driver.appendError("Dupplicated symbol " + name);
		return false;
	}
	m_driver.rootCtx()->m_symTbl.insert(name, pElt);
	return true;
}

inline QString NodeModule::str() {
	return m_prefix;
}

inline NodePtr NodeModule::resolve(const QString symbol, const char *type) {
	auto &symTbl = m_driver.rootCtx()->m_symTbl;
	QString symName;
	
	if (symTbl.contains(m_prefix + symbol)) {
		symName = m_prefix + symbol;
	} else if (symTbl.contains(symbol)) {
		symName = symbol;
	} else {		
		m_driver.appendError("Undefined symbol " + symbol);
		return nullptr;
	}
	NodePtr result = symTbl[symName];
	if (result->map()[KNODE_TYPE].toString().at(0) != type[0]) {
		m_driver.appendError("Invalid symbol " + symName);
		return nullptr;
	}
	return result;
}

}

#endif // IDLPARSER_NODEMODULE_H
