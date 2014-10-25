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

#ifndef IDLPARSER_NODE_H
#define IDLPARSER_NODE_H

#include "ltype.h"

namespace idlparser {
	
class Driver;

class Node: public SharedData {
protected:
	Node();
public:
	virtual ~Node();
	virtual QString str();
	virtual QVariant &val();
	virtual QVariantMap &map();
	virtual QVariantList &list();
	virtual NodePtr resolve(const QString symbol, const char *type);
	virtual Node *insert(const QString &key, const QVariant &value);
	virtual Node *append(const QVariant &value);
	virtual bool append(NodePtr &pElt);
};

inline Node::Node() {}

inline Node::~Node() {}

inline QString Node::str() {
	qt_assert("Assert: Illegal call to QString Node::str()", __FILE__, __LINE__);
	throw 0;
}

inline QVariant& Node::val() {
	qt_assert("Assert: Illegal call to QVariant &Node::val()", __FILE__, __LINE__);
	throw 0;
}

inline QVariantList& Node::list() {
	qt_assert("Assert: Illegal call to QVariantList &Node::list()", __FILE__, __LINE__);
	throw 0;
}

inline QVariantMap& Node::map() {
	qt_assert("Assert: Illegal call to QVariantMap &Node::map()", __FILE__, __LINE__);
	throw 0;
}

inline Node *Node::append(const QVariant &value) {
	qt_assert("Assert: Illegal call to Node *Node::append(const QVariant &)", __FILE__, __LINE__);
	throw 0;
}

inline Node *Node::insert(const QString &key, const QVariant &value) {
	qt_assert("Assert: Illegal call to Node *Node::insert(const QString &, const QVariant &)", __FILE__, __LINE__);
	throw 0;
}

inline bool Node::append(NodePtr &pElt) {
	qt_assert("Assert: Illegal call to bool Node::append(NodePtr &)", __FILE__, __LINE__);
	throw 0;
}

inline NodePtr Node::resolve(const QString symbol, const char *type) {
	qt_assert("Assert: Illegal call to NodePtr Node::resolve(NodePtr &)", __FILE__, __LINE__);
	throw 0;
}

}

#endif // IDLPARSER_NODE_H
