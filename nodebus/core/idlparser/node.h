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
	virtual NodePtr resolve(NodePtr &pSym, const char *type);
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

inline NodePtr Node::resolve(NodePtr &pSym, const char *type) {
	qt_assert("Assert: Illegal call to NodePtr Node::resolve(NodePtr &)", __FILE__, __LINE__);
	throw 0;
}

}

#endif // IDLPARSER_NODE_H
