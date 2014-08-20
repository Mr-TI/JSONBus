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

#ifndef IDLLTYPE_H
#define IDLLTYPE_H

#define YYSTYPE idlparser::LType

#include <QVariant>
#include <QString>
#include <nodebus/core/shareddata.h>
#include <nodebus/core/sharedptr.h>
#include <logger.h>
#include "driver.h"

#define KNODE_TYPE         "T"
#define KNODE_DTYPE        "t"
#define KNODE_SNAME        "n"
#define KNODE_DIRECTION    "d"
#define KNODE_WRITABLE     "w"
#define KNODE_VALUE        "v"
#define KNODE_PARAMS       "p"

#define VTYPE_VOID         'v'
#define VTYPE_ANY          'a'
#define VTYPE_BOOLEAN      'b'
#define VTYPE_BYTE         'o'
#define VTYPE_UINT32       'I'
#define VTYPE_INT32        'i'
#define VTYPE_UINT64       'L'
#define VTYPE_INT64        'l'
#define VTYPE_DOUBLE       'd' 
#define VTYPE_STRING       's'
#define VTYPE_BYTEARRAY    'O'
#define VTYPE_DATETIME     't'

#define NTYPE_DOCUMENT     'D'
#define NTYPE_CONST        'C'
#define NTYPE_ATTR         'A'
#define NTYPE_INTERFACE    'I'
#define NTYPE_METHOD       'M'
#define NTYPE_ENUM         'E'
#define NTYPE_STRUCT       'S'
#define NTYPE_TYPEDEF      'T'
#define NTYPE_PACKAGE      'P'

#define PDIR_IN            'i'
#define PDIR_OUT           'o'
#define PDIR_INOUT         'b'

using namespace NodeBus;

namespace idlparser {
	
class Driver;

class NodeVariant: public Node {
private:
	QVariant m_var;
public:
	NodeVariant(const QVariant &value);
	virtual QString toString();
	virtual QVariant &val();
};

class NodeList: public Node {
private:
	QVariantList m_list;
public:
	virtual QVariantList &list();
	virtual Node *append(const QVariant &value);
};

class NodeMap: public Node {
private:
	QVariantMap m_map;
public:
	virtual QVariantMap &map();
	virtual Node *insert(const QString &key, const QVariant &value);
};

class NodeParams: public Node {
private :
	QHash<QString, bool> params;
	QVariantList m_list;
public:
	virtual QVariantList &list();
	virtual bool appendParam(SharedPtr<Node> &pElt, Driver &driver);
};

class NodeModule: public Node {
public:
	Context(NodePtr &ctx, const QString& name);
	virtual ~Context();
	QString name;
	QString shortName;
	QVariantMap symTbl;
};

class NodeIntf: public Node {
public:
	Context(NodePtr &ctx, const QString& name);
	virtual ~Context();
	QString name;
	QString shortName;
	QVariantMap symTbl;
};

class NodeRoot: public Node {
public:
	Context(NodePtr &ctx, const QString& name);
	virtual ~Context();
	QString name;
	QString shortName;
	QVariantMap symTbl;
};

class LType: public SharedData {
public:
	LType();
	LType(const LType& other);
	virtual ~LType();
	SharedPtr<Node> node;
	QString *str;
	LType& operator= (const LType& other);
};

inline LType::LType(): str(NULL) {
}

inline LType::LType(const LType& other) {
	node = other.node;
	if (other.str) str = new QString(*(other.str));
}

inline LType::~LType() {
	delete str;
}

inline LType& LType::operator=(const LType& other) {
	node = other.node;
	if (other.str) str = new QString(*(other.str));
	return *this;
}

inline bool opexec(SharedPtr<Node> &res, char op, SharedPtr<Node> &op1_n, SharedPtr<Node> &op2_n, Driver &driver) {
	QVariant &op1 = op1_n->val();
	QVariant &op2 = op2_n->val();
	if (!op1.canConvert(QVariant::Double)) {
		return driver.appendError("Error: invalid operand " + Logger::dump(op1));
	} else if (!op2.canConvert(QVariant::Double)) {
		return driver.appendError("Error: invalid operand " + Logger::dump(op2));
	} else if (op1.type() == QVariant::Double || op2.type() == QVariant::Double) {
		switch (op) {
			case '+':
				res = new NodeVariant(op1.toDouble() + op1.toDouble());
				break;
			case '-':
				res = new NodeVariant(op1.toDouble() + op1.toDouble());
				break;
			case '*':
				res = new NodeVariant(op1.toDouble() + op1.toDouble());
				break;
			case '/':
				res = new NodeVariant(op1.toDouble() + op1.toDouble());
				break;
			case '%':
				res = new NodeVariant(op1.toLongLong() % op1.toLongLong());
				break;
			default:
				return driver.appendError("Error: invalid operator " + char(op));
		}
		
	} else {
		switch (op) {
			case '+':
				res = new NodeVariant(op1.toLongLong() + op1.toLongLong());
				break;
			case '-':
				res = new NodeVariant(op1.toLongLong() + op1.toLongLong());
				break;
			case '*':
				res = new NodeVariant(op1.toLongLong() + op1.toLongLong());
				break;
			case '/':
				res = new NodeVariant(op1.toLongLong() + op1.toLongLong());
				break;
			case '%':
				res = new NodeVariant(op1.toLongLong() % op1.toLongLong());
				break;
			default:
				return driver.appendError("Error: invalid operator " + char(op));
		}
	}
	return true;
}

inline Node::Node() {}

inline Node::~Node() {}

inline QString Node::toString() {
	throw Exception("Fatal: Illegal call to QString Node::toString()");
}

inline QVariant& Node::val() {
	throw Exception("Fatal: Illegal call to QVariant &Node::val()");
}

inline QVariantList& Node::list() {
	throw Exception("Fatal: Illegal call to QVariantList &Node::list()");
}

inline QVariantMap& Node::map() {
	throw Exception("Fatal: Illegal call to QVariantMap &Node::map()");
}

inline Node *Node::append(const QVariant &value) {
	throw Exception("Fatal: Illegal call to Node *Node::append(const QVariant &)");
}

inline Node *Node::insert(const QString &key, const QVariant &value) {
	throw Exception("Fatal: Illegal call to Node *Node::insert(const QString &, const QVariant &)");
}

inline bool Node::appendParam(SharedPtr<Node> &pElt, Driver &driver) {
	throw Exception("Fatal: Illegal call to bool Node::appendParam(SharedPtr<Node> &, QString &)");
}

inline NodeVariant::NodeVariant(const QVariant& value): m_var(value) {
}

inline QString NodeVariant::toString() {
	return m_var.toString();
}

inline QVariant& NodeVariant::val() {
	return m_var;
}

inline QVariantList& NodeList::list() {
	return m_list;
}

inline Node *NodeList::append(const QVariant &value) {
	m_list.append(value);
	return this;
}

inline QVariantMap& NodeMap::map() {
	return m_map;
}

inline Node *NodeMap::insert(const QString &key, const QVariant &value) {
	m_map.insert(key, value);
	return this;
}

inline bool NodeParams::appendParam(SharedPtr<Node> &pElt, Driver &driver) {
	QVariantMap &param = pElt->map();
	QString k = param["n"].toString();
	if (params.contains(k)) {
		driver.appendError("Dupplicate parameter " + k);
		return false;
	}
	params[k] = true;
	m_list.append(param);
	return true;
}

inline QVariantList& NodeParams::list() {
	return m_list;
}

inline Context::Context(NodePtr& ctx, const QString& shortName)
: name(shortName), shortName(ctx == nullptr ? shortName : ctx->name + "::" + shortName) {
	
}

inline Context::~Context() {
	
}

}

#endif // IDLLTYPE_H
