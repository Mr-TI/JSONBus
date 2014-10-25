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
	QString name = elt[NODE_KEY_NAME].toString();
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
	if (result->map()[NODE_KEY_NODE_TYPE].toString().at(0) != type[0]) {
		m_driver.appendError("Invalid symbol " + symName);
		return nullptr;
	}
	return result;
}

}

#endif // IDLPARSER_NODEMODULE_H
