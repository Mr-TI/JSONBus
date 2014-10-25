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

#ifndef IDLPARSER_NODEVARIANT_H
#define IDLPARSER_NODEVARIANT_H

#include "ltype.h"
#include "node.h"

namespace idlparser {

class NodeVariant: public Node {
private:
	QVariant m_var;
public:
	NodeVariant(const QVariant &value);
	virtual QString str();
	virtual QVariant &val();
};

inline NodeVariant::NodeVariant(const QVariant& value): m_var(value) {
}

inline QString NodeVariant::str() {
	return m_var.toString();
}

inline QVariant& NodeVariant::val() {
	return m_var;
}

}

#endif // IDLPARSER_NODEVARIANT_H
