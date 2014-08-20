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

#ifndef IDLPARSER_DRIVER_H
#define IDLPARSER_DRIVER_H

#include <QVariant>
#include <QStack>
#include <QMap>
#include <QStringList>
#include <stdio.h>

namespace idlparser {

class Parser;
class Scanner;
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
	virtual bool appendParam(SharedPtr<Node> &pElt, Driver &driver);
};

typedef SharedPtr<Node> NodePtr;

/**
 * @brief IDL parser driver
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class Driver {
public:
	Driver(const QString &filename, NodePtr shared);
	~Driver();
	QVariant parse();
	bool appendError(const QString &message);
private:
	friend class Parser;
	friend class Scanner;
	QStringList m_errors;
	FILE *m_stream;
	QString m_filename;
	Scanner &scanner;
	Parser &parser;
	QVariant result;
	NodePtr shared;
	NodePtr context;
	QStack<NodePtr> nodeStack;
	QVariantMap synTbl;
	void push(NodePtr node);
	void pop();
	bool addSym(const QVariant &node);
};

inline void Driver::intfBegin(const QString& name) {
	stack.push(context);
	context = new Context(context, name);
}

inline QVariantMap Driver::intfEnd() {
	QVariantMap node;
	node.insert("n", context->shortName);
	node.insert("m", context->symTbl.values());
	context = stack.pop();
	return node;
}

inline void Driver::moduleBegin(const QString& name) {
	stack.push(context);
	context = new Context(context, name);
}

inline void Driver::moduleEnd() {
	context = stack.pop();
}

inline bool Driver::addSym(const QVariant& symbol, const QVariant &node) {
	QString shortName = symbol.toString();
	QString name = (context == nullptr ? "" : context->name + "::") + shortName;
	if (symTbl.contains(name)) {
		return appendError("Dupplicate symbol " + name);
	}
	if (context != nullptr) {
		context->symTbl.insert(shortName, node);
	}
	symTbl.insert(name, node);
	return true;
}


}

#endif // IDLPARSER_DRIVER_H
