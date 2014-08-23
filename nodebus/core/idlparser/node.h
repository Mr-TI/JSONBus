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
	virtual NodePtr resolve(NodePtr &pSym, char type);
	virtual Node *insert(const QString &key, const QVariant &value);
	virtual Node *append(const QVariant &value);
	virtual bool append(NodePtr &pElt);
};

inline Node::Node() {}

inline Node::~Node() {}

inline QString Node::str() {
	throw Exception("Assert: Illegal call to QString Node::str()");
}

inline QVariant& Node::val() {
	throw Exception("Assert: Illegal call to QVariant &Node::val()");
}

inline QVariantList& Node::list() {
	throw Exception("Assert: Illegal call to QVariantList &Node::list()");
}

inline QVariantMap& Node::map() {
	throw Exception("Assert: Illegal call to QVariantMap &Node::map()");
}

inline Node *Node::append(const QVariant &value) {
	throw Exception("Assert: Illegal call to Node *Node::append(const QVariant &)");
}

inline Node *Node::insert(const QString &key, const QVariant &value) {
	throw Exception("Assert: Illegal call to Node *Node::insert(const QString &, const QVariant &)");
}

inline bool Node::append(NodePtr &pElt) {
	throw Exception("Assert: Illegal call to bool Node::append(NodePtr &)");
}

inline NodePtr Node::resolve(NodePtr &pSym, char type) {
	throw Exception("Assert: Illegal call to NodePtr Node::resolve(NodePtr &)");
}

}

#endif // IDLPARSER_NODE_H
