/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
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
#include <QDir>
#include <QStack>
#include <QMap>
#include <QStringList>
#include <QString>
#include <stdio.h>

namespace idlparser {

class Parser;
class Scanner;
class Driver;
class Node;
class NodeRoot;

typedef SharedPtr<Node> NodePtr;

/**
 * @brief IDL parser driver
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class Driver {
public:
	~Driver();
	bool appendError(const QString &message);
	bool include(const QString &filename);
	SharedPtr<NodeRoot> rootCtx();
	NodePtr curCtx();
	static QVariant parse(const QString &filename);
private:
	Driver(const QString &filename, Scanner &scanner, SharedPtr<NodeRoot> shared = nullptr);
	bool setOpResult(NodePtr &pVar, NodePtr &pRes);
	bool opexec(NodePtr &res, char op, NodePtr &op1_n, NodePtr &op2_n);
	friend class Parser;
	friend class Scanner;
	QVariantList m_localElts;
	FILE *m_stream;
	QString m_filename;
	QDir m_filedir;
	Scanner &m_scanner;
	SharedPtr<NodeRoot> m_rootCtx;
	NodePtr m_curCtx;
	QStack<NodePtr> m_nodeStack;
	void push(NodePtr node);
	void pop();
	bool addSym(const QVariant &node);
};

}

#endif // IDLPARSER_DRIVER_H
