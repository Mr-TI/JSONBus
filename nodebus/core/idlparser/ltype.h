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

using namespace NodeBus;

namespace idlparser {
	
class Driver;

class Node: public SharedData {
private:
	void *data;
	Node();
public:
	Node(const QVariant &variant);
	~Node();
	QString toString();
	QVariant &val();
	QVariantMap &map();
	QVariantList &list();
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
	~LType();
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

inline Node* op_plus(const QVariant &op1, const QVariant &op2) {
	if (!op1.canConvert(QVariant::Double) || !op2.canConvert(QVariant::Double)) {
		return NULL;
	} else if (op1.type() == QVariant::Double || op2.type() == QVariant::Double) {
		return new Node(op1.toDouble() + op1.toDouble());
	} else {
		return new Node(op1.toLongLong() + op1.toLongLong());
	}
}

inline Node* op_minus(const QVariant &op1, const QVariant &op2) {
	if (!op1.canConvert(QVariant::Double) || !op2.canConvert(QVariant::Double)) {
		return NULL;
	} else if (op1.type() == QVariant::Double || op2.type() == QVariant::Double) {
		return new Node(op1.toDouble() - op1.toDouble());
	} else {
		return new Node(op1.toLongLong() - op1.toLongLong());
	}
}

inline Node* op_mult(const QVariant &op1, const QVariant &op2) {
	if (!op1.canConvert(QVariant::Double) || !op2.canConvert(QVariant::Double)) {
		return NULL;
	} else if (op1.type() == QVariant::Double || op2.type() == QVariant::Double) {
		return new Node(op1.toDouble() * op1.toDouble());
	} else {
		return new Node(op1.toLongLong() * op1.toLongLong());
	}
}

inline Node* op_divid(const QVariant &op1, const QVariant &op2) {
	if (!op1.canConvert(QVariant::Double) || !op2.canConvert(QVariant::Double)) {
		return NULL;
	} else if (op1.type() == QVariant::Double || op2.type() == QVariant::Double) {
		return new Node(op1.toDouble() / op1.toDouble());
	} else {
		return new Node(op1.toLongLong() / op1.toLongLong());
	}
}

inline Node* op_rest(const QVariant &op1, const QVariant &op2) {
	if (!op1.canConvert(QVariant::Double) || !op2.canConvert(QVariant::Double)
		|| op1.type() == QVariant::Double || op2.type() == QVariant::Double) {
		return NULL;
	} else {
		return new Node(op1.toLongLong() % op1.toLongLong());
	}
}

}

#endif // IDLLTYPE_H
