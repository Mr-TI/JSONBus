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

#ifndef IDLPARSER_NODEINTF_H
#define IDLPARSER_NODEINTF_H

#include "ltype.h"
#include "node.h"
#include "nodevariant.h"
#include "noderoot.h"

namespace idlparser {

class NodeIntf: public Node {
private :
	Driver &m_driver;
public:
	NodeIntf(Driver &driver, NodePtr &pSym, NodePtr &pParents);
	virtual NodePtr resolve(NodePtr &pSym, char type);
	virtual bool append(NodePtr &pElt);
	virtual QString str();
	QString m_name;
	QString m_prefix;
	QVariantMap m_symTbl;
};

inline NodeIntf::NodeIntf(Driver &driver, NodePtr &pSym, NodePtr &pParents)
: m_driver(driver), m_name(pSym->str()), m_prefix(driver.shared()->str() + m_name + "::") {
	
}

inline bool NodeIntf::append(NodePtr &pElt) {
	QVariantMap elt = pElt->map();
	QString name = elt[KNODE_SNAME].toString();
	if (m_symTbl.contains(name)) {
		m_driver.appendError("Dupplicate symbol " + name);
		return false;
	}
	m_symTbl.insert(name, elt);
	return true;
}

inline QString NodeIntf::str() {
	return m_prefix;
}

inline NodePtr NodeIntf::resolve(NodePtr &pSym, char type) {
	QString name = pSym->str();
	QVariant result;
	if (m_symTbl.contains(name)) {
		result = m_symTbl[name];
	} else if (m_driver.shared().cast<NodeRoot>()->m_symTbl.contains(name)) {
		result = m_driver.shared().cast<NodeRoot>()->m_symTbl[name]/*.toMap()[KNODE_VALUE]*/;
	} else {
		m_driver.appendError("Undefined symbol " + name);
		return new NodeVariant(0);
	}
	QVariantMap mNode = result.toMap();
	if (mNode[KNODE_TYPE].toChar() == type) {
		return new NodeVariant(mNode[KNODE_VALUE]);
	} else {
		m_driver.appendError("Invalid symbol " + name);
		return new NodeVariant(0);
	}
}

}

#endif // IDLPARSER_NODEINTF_H
