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

#ifndef IDLPARSER_NODELIST_H
#define IDLPARSER_NODELIST_H

#include "ltype.h"
#include "node.h"

namespace idlparser {

class NodeList: public Node {
private:
	QVariantList m_list;
public:
	virtual QVariantList &list();
	virtual Node *append(const QVariant &value);
};

inline QVariantList& NodeList::list() {
	return m_list;
}

inline Node *NodeList::append(const QVariant &value) {
	m_list.append(value);
	return this;
}

}

#endif // IDLPARSER_NODELIST_H
