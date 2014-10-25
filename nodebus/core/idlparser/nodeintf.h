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

#ifndef IDLPARSER_NODEINTF_H
#define IDLPARSER_NODEINTF_H

#include "ltype.h"
#include "nodemodule.h"

namespace idlparser {

class NodeIntf: public NodeModule {
private:
	QVariantMap m_infos;
	bool m_finalized;
public:
	NodeIntf(Driver &driver, NodePtr &pSym, NodePtr &pParents);
	virtual QVariantMap &map();
	virtual bool append(NodePtr &pElt);
	QString m_name;
	QVariantList m_members;
	QMap<QString, SharedPtr<NodeIntf> > m_parents;
};

inline NodeIntf::NodeIntf(Driver &driver, NodePtr &pSym, NodePtr &pParents)
: NodeModule(driver, pSym), m_finalized(false), m_name(pSym->str()) {
	QString fullName = driver.curCtx()->str() + pSym->str();
	if (driver.rootCtx()->m_symTbl.contains(fullName)) {
		m_driver.appendError("Dupplicated symbol " + fullName);
	}
	QVariantList parentList = pParents->list();
	QVariantList parents;
	for (auto parent: pParents->list()) {
		SharedPtr<NodeIntf> pIntf = driver.curCtx()->resolve(parent.toString(), TYPE_NODE_INTERFACE);
		if (pIntf == nullptr) continue;
		QString parentName = pIntf->map()[NODE_KEY_NAME].toString();
		m_parents[parentName] = pIntf;
		auto &gpMap = pIntf->m_parents;
		for (auto itGP = gpMap.begin(); itGP != gpMap.end(); itGP++) {
			m_parents.insert(itGP.key(), itGP.value());
		}
		parents.append(parentName);
	}
	m_infos.insert(NODE_KEY_NAME, fullName);
	m_infos.insert(NODE_KEY_PARENTS, parents);
}

inline bool NodeIntf::append(NodePtr &pElt) {
	QVariantMap elt = pElt->map();
	QString name = m_prefix + elt[NODE_KEY_NAME].toString();
	if (m_driver.rootCtx()->m_symTbl.contains(name)) {
		m_driver.appendError("Dupplicated symbol " + name);
		return false;
	}
	m_driver.rootCtx()->m_symTbl.insert(name, pElt);
	m_members.append(elt);
	return true;
}

}

#endif // IDLPARSER_NODEINTF_H
