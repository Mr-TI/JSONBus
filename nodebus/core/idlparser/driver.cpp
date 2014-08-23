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

#include "noderoot.h"
#include <parser.hh>
#include "scanner.h"
#include "driver.h"

#include <nodebus/core/parser.h>
using namespace NodeBus;

namespace idlparser {

Driver::Driver(const QString &filename, Scanner &scanner, SharedPtr<NodeRoot> shared)
: m_filename(filename), m_filedir(QFileInfo(filename).dir()), m_scanner(scanner), 
m_rootCtx(shared == nullptr ? new NodeRoot(*this): shared), m_curCtx(m_rootCtx) {
}

Driver::~Driver() {
}

bool Driver::appendError(const QString& message) {
	m_rootCtx->m_errors.append("File: " + m_filename + ", line: " + 
		QString::number(m_scanner.lineno()) + ", column: " + 
		QString::number(m_scanner.YYLeng()) + ", " + message);
	return m_rootCtx->m_errors.size() < 20;
}

QVariant Driver::parse(const QString &filename) {
	QString filePath = QDir::current().absoluteFilePath(filename);
	Scanner scanner(filename);
	Driver driver(filename, scanner);
	Parser parser(driver);
	parser.parse();
	if (!driver.rootCtx()->m_errors.isEmpty()) {
		throw ErrorParserException(driver.rootCtx()->m_errors.join("\n"));
	}
	return 0;
}

void Driver::pop() {
	m_curCtx = m_nodeStack.pop();
}

void Driver::push(NodePtr node) {
	m_nodeStack.push(m_curCtx);
	m_curCtx = node;
}

bool Driver::include(const QString& filename) {
	QString filePath = m_filedir.absoluteFilePath(filename);
	Scanner scanner(filename);
	Driver driver(filename, scanner, m_rootCtx);
	Parser parser(driver);
	parser.parse();
	return m_rootCtx->m_errors.isEmpty();
}

}

