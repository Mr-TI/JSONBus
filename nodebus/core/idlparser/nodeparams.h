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

#ifndef IDLPARSER_NODEPARAMS_H
#define IDLPARSER_NODEPARAMS_H

#include "ltype.h"
#include "node.h"

namespace idlparser {

class NodeParams: public Node {
private :
	QHash<QString, bool> m_params;
	QVariantList m_list;
	Driver &m_driver;
public:
	NodeParams(Driver &driver);
	virtual QVariantList &list();
	virtual bool append(SharedPtr<Node> &pElt);
};

inline NodeParams::NodeParams(Driver& driver): m_driver(driver) {
}

inline bool NodeParams::append(SharedPtr<Node> &pElt) {
	QVariantMap &param = pElt->map();
	QString k = param[NODE_KEY_NAME].toString();
	if (m_params.contains(k)) {
		m_driver.appendError("Dupplicated parameter " + k);
		return false;
	}
	m_params.insert(k, true);
	m_list.append(param);
	return true;
}

inline QVariantList& NodeParams::list() {
	return m_list;
}

}

#endif // IDLPARSER_NODEPARAMS_H
