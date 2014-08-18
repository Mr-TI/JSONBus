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
private:
	void *data;
	Node();
public:
	Node(const QVariant &variant);
	virtual ~Node();
	QString toString();
	QVariant &val();
	QVariantMap &map();
	QVariantList &list();
	Node *insert(const QString &key, const QVariant &value);
	Node *append(const QVariant &value);
	static Node *newMap();
	static Node *newMap(const QString &key, const QVariant &value);
	static Node *newList();
	static Node *newList(const QVariant &value);
};

inline Node::Node() {}

inline Node::Node(const QVariant& variant) {
	data = new QVariant(variant);
}

inline QString Node::toString() {
	return ((QVariant*)data)->toString();
}

inline QVariant& Node::val() {
	return *((QVariant*)data);
}

inline QVariantList& Node::list() {
	return *((QVariantList*)data);
}

inline QVariantMap& Node::map() {
	return *((QVariantMap*)data);
}

inline Node *Node::append(const QVariant &value) {
	((QVariantList*)data)->append(value);
	return this;
}

inline Node *Node::insert(const QString &key, const QVariant &value) {
	((QVariantMap*)data)->insert(key, value);
	return this;
}

inline Node* Node::newList(const QVariant& value) {
	Node *node = new Node();
	node->data = new QVariantList();
	node->list().append(value);
	return node;
}

inline Node* Node::newList() {
	Node *node = new Node();
	node->data = new QVariantList();
	return node;
}

inline Node* Node::newMap(const QString& key, const QVariant& value) {
	Node *node = new Node();
	node->data = new QVariantMap();
	node->map().insert(key, value);
	return node;
}

inline Node* Node::newMap() {
	Node *node = new Node();
	node->data = new QVariantMap();
	return node;
}

inline Node::~Node() {
	if (dynamic_cast<QVariant*>((QObject*)data)) {
		delete (QVariant*)data;
	} else if (dynamic_cast<QVariantMap*>((QObject*)data)) {
		delete (QVariantMap*)data;
	} else if (dynamic_cast<QVariantList*>((QObject*)data)) {
		delete (QVariantList*)data;
	}
}

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

class BlockCtx: public SharedData {
public:
	BlockCtx(SharedPtr<BlockCtx> variant);
	virtual ~BlockCtx();
	QString name;
	QVariantMap symTbl;
};

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
