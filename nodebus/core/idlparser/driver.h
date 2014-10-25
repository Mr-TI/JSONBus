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
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */
class Driver {
public:
	~Driver();
	bool appendError(const QString &message);
	bool include(const QString &filename);
	SharedPtr<NodeRoot> rootCtx();
	NodePtr curCtx();
	static QVariant parse(const QString &filename);
	NodePtr relolveConsVal(NodePtr pSyn);
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
