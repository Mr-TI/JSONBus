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
#include "nodeintf.h"

#include <nodebus/core/parser.h>
#include <logger.h>
using namespace NodeBus;

namespace idlparser {

Driver::Driver(const QString &filename, Scanner &scanner, SharedPtr<NodeRoot> shared)
: m_filename(filename), m_filedir(QFileInfo(filename).dir()), m_scanner(scanner), 
m_rootCtx(shared == nullptr ? new NodeRoot(*this): shared), m_curCtx(m_rootCtx) {
	m_rootCtx->m_fileList.insert(filename, true);
}

Driver::~Driver() {
}

bool Driver::appendError(const QString& message) {
	m_rootCtx->m_errors.append(m_filename + ":" + 
		QString::number(m_scanner.lineno()) + ": " + message);
	return m_rootCtx->m_errors.size() < 20;
}

QVariant Driver::parse(const QString &filename) {
	QString filePath = QFileInfo(filename).absoluteFilePath();
	Scanner scanner(filePath);
	Driver driver(filePath, scanner);
	Parser parser(driver);
	parser.parse();
	if (!driver.rootCtx()->m_errors.isEmpty()) {
		throw ErrorParserException(driver.rootCtx()->m_errors.join("\n"));
	}
	return driver.m_localElts;
}

void Driver::pop() {
	if (m_curCtx.instanceof<NodeIntf>()) {
		m_localElts.append(m_curCtx->map());
	}
	m_curCtx = m_nodeStack.pop();
}

void Driver::push(NodePtr node) {
	m_nodeStack.push(m_curCtx);
	m_curCtx = node;
}

bool Driver::include(const QString& filename) {
	QString filePath = m_filedir.absoluteFilePath(filename);
	if (m_rootCtx->m_fileList.contains(filename)) {
		// already parsed
		return true;
	}
	Scanner scanner(filePath);
	Driver driver(filePath, scanner, m_rootCtx);
	Parser parser(driver);
	parser.parse();
	return m_rootCtx->m_errors.isEmpty();
}

}

