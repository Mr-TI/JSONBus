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

#ifndef IDLPARSER_NODEROOT_H
#define IDLPARSER_NODEROOT_H

#include "ltype.h"
#include "node.h"

namespace idlparser {

class NodeRoot: public Node {
private :
	Driver &m_driver;
public:
	NodeRoot(Driver &driver);
	virtual NodePtr resolve(const QString symbol, const char *type);
	virtual bool append(NodePtr &pElt);
	virtual QString str();
	QMap<QString, bool> m_fileList;
	QMap<QString, NodePtr> m_symTbl;
	QStringList m_errors;
};

inline NodeRoot::NodeRoot(Driver& driver): m_driver(driver) {

}

inline bool NodeRoot::append(NodePtr &pElt) {
	QVariantMap elt = pElt->map();
	QString name = elt[NODE_KEY_NAME].toString();
	if (m_symTbl.contains(name)) {
		m_driver.appendError("Dupplicated symbol " + name);
		return false;
	}
	m_symTbl.insert(name, pElt);
	return true;
}

inline QString NodeRoot::str() {
	static const QString empty;
	return empty; // empty prefix
}

inline NodePtr NodeRoot::resolve(const QString symbol, const char *type) {
	NodePtr result = m_symTbl.value(symbol);
	if (result == nullptr) {
		m_driver.appendError("Undefined symbol " + symbol);
		return result;
	}
	if (result->map()[NODE_KEY_NODE_TYPE].toString().at(0) != type[0]) {
		m_driver.appendError("Invalid symbol " + symbol);
		return nullptr;
	}
	return result;
}

}

#endif // IDLPARSER_NODEROOT_H
