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

using namespace NodeBus;

namespace idlparser {
	
class Driver;

class Node: public SharedData {
protected:
	Node();
public:
	virtual ~Node();
	virtual QString toString();
	virtual QVariant &val();
	virtual QVariantMap &map();
	virtual QVariantList &list();
	virtual Node *insert(const QString &key, const QVariant &value);
	virtual Node *append(const QVariant &value);
	virtual bool appendParam(SharedPtr<Node> &pElt, QString &lastError);
};

inline Node::Node() {}

inline Node::Node(const QVariant& variant) {
	data = new QVariant(variant);
}

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
	throw Exception("Fatal: Illegal call to Node::()");
}

class NodeVariant: public Node {
private:
	QVariant m_var;
public:
	Node();
	virtual ~Node();
	virtual QString toString();
	virtual QVariant &val();
	virtual QVariantMap &map();
	virtual QVariantList &list();
	virtual Node *insert(const QString &key, const QVariant &value);
	virtual Node *append(const QVariant &value);
	virtual bool appendParam(SharedPtr<Node> &pElt, QString &lastError);
};

inline QString NodeVariant::toString() {
	return m_var.toString();
}

inline QVariant& NodeVariant::val() {
	return m_var;
}

class NodeList: public Node {
private:
	QVariantList m_list;
public:
	Node();
	virtual ~Node();
	virtual QVariantList &list();
	virtual Node *append(const QVariant &value);
};

inline QVariantList& NodeList::list() {
	return m_list;
}

inline Node *NodeList::append(const QVariant &value) {
	m_list->append(value);
	return this;
}

class NodeMap: public Node {
private:
	QVariantList m_map;
public:
	Node();
	virtual QVariantMap &map();
	virtual Node *insert(const QString &key, const QVariant &value);
};

inline QVariantMap& NodeMap::map() {
	return m_map;
}

inline Node *NodeMap::insert(const QString &key, const QVariant &value) {
	m_map.insert(key, value);
	return this;
}

class NodeParams: public Node {
private :
	QHash<QString, bool> params;
	QVariantList m_list;
public:
	NodeParams();
	bool appendParam(SharedPtr<Node> &pElt, QString &lastError);
};

inline NodeParams::NodeParams() {
	data = new QVariantList();
}

inline bool NodeParams::appendParam(SharedPtr<Node> &pElt, QString &lastError) {
	QVariantMap &param = pElt->map();
	QString k = param["n"].toString();
	if (params.contains(k)) {
		lastError = "Dupplicate parameter " + k;
		return false;
	}
	params[k] = true;
	m_list->append(param);
	return true;
}

inline QVariantList& NodeList::list() {
	return m_list;
}

class BlockCtx: public SharedData {
public:
	BlockCtx(SharedPtr<BlockCtx> variant);
	virtual ~BlockCtx();
	QString name;
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

inline bool opexec(SharedPtr<Node> &res, char op, SharedPtr<Node> &op1_n, SharedPtr<Node> &op2_n, QString &lastError) {
	QVariant &op1 = op1_n->val();
	QVariant &op2 = op2_n->val();
	if (!op1.canConvert(QVariant::Double)) {
		lastError = "Error: invalid operand " + Logger::dump(op1) ;
		return false;
	} else if (!op2.canConvert(QVariant::Double)) {
		lastError = "Error: invalid operand " + Logger::dump(op2) ;
		return false;
	} else if (op1.type() == QVariant::Double || op2.type() == QVariant::Double) {
		switch (op) {
			case '+':
				res = new Node(op1.toDouble() + op1.toDouble());
				break;
			case '-':
				res = new Node(op1.toDouble() + op1.toDouble());
				break;
			case '*':
				res = new Node(op1.toDouble() + op1.toDouble());
				break;
			case '/':
				res = new Node(op1.toDouble() + op1.toDouble());
				break;
			case '%':
				res = new Node(op1.toLongLong() % op1.toLongLong());
				break;
			default:
				lastError = "Error: invalid operator " + char(op) ;
				return false;
		}
		
	} else {
		switch (op) {
			case '+':
				res = new Node(op1.toLongLong() + op1.toLongLong());
				break;
			case '-':
				res = new Node(op1.toLongLong() + op1.toLongLong());
				break;
			case '*':
				res = new Node(op1.toLongLong() + op1.toLongLong());
				break;
			case '/':
				res = new Node(op1.toLongLong() + op1.toLongLong());
				break;
			case '%':
				res = new Node(op1.toLongLong() % op1.toLongLong());
				break;
			default:
				lastError = "Error: invalid operator " + char(op) ;
				return false;
		}
	}
	return true;
}

inline bool checkParamList(SharedPtr<Node> &pRet, SharedPtr<Node> &pList, QString &lastError) {
	QVariantList &list = pRet->list();
	QHash<QString, bool> params;
	for (auto it = list.begin(); it != list.end(); it++) {
		QVariantMap elt = it->toMap();
		QString k = elt["n"].toString();
		if (params.contains(k)) {
			lastError = "Dupplicate parameter " + k;
			return false;
		}
		params[k] = true;
	}
	pRet = pList;
	return true;
}

}

#endif // IDLLTYPE_H
