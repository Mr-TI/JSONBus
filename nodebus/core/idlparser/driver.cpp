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

#include "ltype.h"
#include <parser.hh>
#include "scanner.h"
#include "driver.h"

#include <nodebus/core/parser.h>
using namespace NodeBus;

namespace idlparser {

inline FILE *__idlparser_fopen(const QString& filename) {
	return NULL;
}

Driver::Driver(const QString& filename, NodePtr shared)
		: m_stream(__idlparser_fopen(filename)), m_scanner(*new Scanner(filename)),
		m_parser(*new Parser(*this)){
}

Driver::~Driver() {
	delete &m_parser;
	delete &m_scanner;
}

bool Driver::appendError(const QString& message) {
	m_errors.append("File: " + m_filename + ", line: " + 
		QString::number(m_scanner.lineno()) + ", column: " + 
		QString::number(m_scanner.YYLeng()) + ", " + message);
	return m_errors.size() < 20;
}

QVariant Driver::parse() {
	result = QVariant();
	m_scanner.resetPos();
	m_parser.parse();
	if (!m_errors.isEmpty()) {
		throw ErrorParserException(m_errors.join("\n"));
	}
	return result;
}

void Driver::pop() {

}

void Driver::push(NodePtr node) {

}

bool Driver::include(const QString& filename) {
	return false;
}

}

