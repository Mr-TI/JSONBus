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
#include "nodemodule.h"

namespace idlparser {

class NodeIntf: public NodeModule {
public:
	NodeIntf(Driver &driver, NodePtr &pSym, NodePtr &pParents);
	virtual bool append(NodePtr &pElt);
	QString m_name;
};

inline NodeIntf::NodeIntf(Driver &driver, NodePtr &pSym, NodePtr &pParents)
: NodeModule(driver, pSym), m_name(pSym->str()) {
	
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

}

#endif // IDLPARSER_NODEINTF_H
