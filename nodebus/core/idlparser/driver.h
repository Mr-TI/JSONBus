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
	virtual bool append(SharedPtr<Node> &pElt);
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
	Driver(const QString &filename, NodePtr shared=nullptr);
	~Driver();
	QVariant parse();
	bool appendError(const QString &message);
	bool include(const QString &filename);
private:
	friend class Parser;
	friend class Scanner;
	QStringList m_errors;
	FILE *m_stream;
	QString m_filename;
	Scanner &m_scanner;
	Parser &m_parser;
	NodePtr m_shared;
	NodePtr m_context;
	QStack<NodePtr> m_nodeStack;
	void push(NodePtr node);
	void pop();
	bool addSym(const QVariant &node);
};

}

#endif // IDLPARSER_DRIVER_H
