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
	QString filePath = QFileInfo(filename).canonicalFilePath();
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
	m_curCtx = m_nodeStack.pop();
}

void Driver::push(NodePtr node) {
	m_nodeStack.push(m_curCtx);
	m_curCtx = node;
}

bool Driver::include(const QString& filename) {
	QString filePath = QFileInfo(m_filedir.absoluteFilePath(filename)).canonicalFilePath();
	if (m_rootCtx->m_fileList.contains(filePath)) {
		// already parsed
		return true;
	}
	try {
		Scanner scanner(filePath);
		Driver driver(filePath, scanner, m_rootCtx);
		Parser parser(driver);
		parser.parse();
	} catch(IOException &e) {
		appendError(e.message());
	}
	return m_rootCtx->m_errors.isEmpty();
}

inline QString type2str(QVariant::Type type) {
	switch (type) {
		case QVariant::Invalid:
			return "an invalid/unspecified type";
		case QVariant::Bool:
			return "a boolean";
		case QVariant::Char:
			return "an octet/byte";
		case QVariant::UInt:
			return "an usigned 32 bits integer";
		case QVariant::Int:
			return "a signed 32 bits integer";
		case QVariant::ULongLong:
			return "an usigned 64 bits integer";
		case QVariant::LongLong:
			return "a signed 64 bits integer";
		case QVariant::Double:
			return "a double presision float";
		case QVariant::String:
			return "a string";
		case QVariant::BitArray:
			return "an octet/byte array";
		case QVariant::DateTime:
			return "a date/time";
		default:
			return "an unknow";
	}
}

bool Driver::setOpResult(NodePtr &pVar, NodePtr &pRes) {
	QVariant &res = pRes->val();
	QVariantMap &var = pVar->map();
	QVariant::Type opDataType;
	QString opDataTypeStr;
	switch (var[KNODE_DTYPE].toString()[0].toAscii()) {
		case VTYPE_VOID[0]:
			return appendError("Error: invalid variable type void");
		case VTYPE_ANY[0]:
			opDataType = QVariant::Invalid;
			break;
		case VTYPE_BOOLEAN[0]:
			opDataType = QVariant::Bool;
			break;
		case VTYPE_BYTE[0]:
			opDataType = QVariant::Char;
			break;
		case VTYPE_UINT32[0]:
			opDataType = QVariant::UInt;
			break;
		case VTYPE_INT32[0]:
			opDataType = QVariant::Int;
			break;
		case VTYPE_UINT64[0]:
			opDataType = QVariant::ULongLong;
			break;
		case VTYPE_INT64[0]:
			opDataType = QVariant::LongLong;
			break;
		case VTYPE_DOUBLE[0]:
			opDataType = QVariant::Double;
			break;
		case VTYPE_STRING[0]:
			opDataType = QVariant::String;
			break;
		case VTYPE_BYTEARRAY[0]:
			opDataType = QVariant::BitArray;
			break;
		case VTYPE_DATETIME[0]:
			opDataType = QVariant::DateTime;
			break;
		default:
			return appendError("Error: invalid variable type");
			break;
	}
	if (!res.convert(opDataType)) {
		return appendError("Error: the result cannot be converted to " + type2str(opDataType));
	}
	var.insert(KNODE_VALUE, res);
	return true;
}

bool Driver::opexec(NodePtr &res, char op, NodePtr &op1_n, NodePtr &op2_n) {
	QVariant &op1 = op1_n->val();
	QVariant &op2 = op2_n->val();
	if (!op1.canConvert(QVariant::Double) && !op1.canConvert(QVariant::Int) && op1.type() != QVariant::String) {
		return appendError("Error: invalid operand " + Logger::dump(op1));
	} else if (!op2.canConvert(QVariant::Double) && !op2.canConvert(QVariant::Int) && op1.type() != QVariant::String) {
		return appendError("Error: invalid operand " + Logger::dump(op2));
	} else if (op1.type() == QVariant::String || op2.type() == QVariant::String) {
		switch (op) {
			case '+':
				res = new NodeVariant(op1.toString() + op2.toString());
				break;
			case '-':
			case '*':
			case '/':
			case '%':
				return appendError("Error: invalid operator fot string value: " + char(op));
				break;
			default:
				return appendError("Error: invalid operator " + char(op));
		}
	} else if (op1.type() == QVariant::Double || op2.type() == QVariant::Double) {
		switch (op) {
			case '+':
				res = new NodeVariant(op1.toDouble() + op2.toDouble());
				break;
			case '-':
				res = new NodeVariant(op1.toDouble() - op2.toDouble());
				break;
			case '*':
				res = new NodeVariant(op1.toDouble() * op2.toDouble());
				break;
			case '/':
				res = new NodeVariant(op1.toDouble() / op2.toDouble());
				break;
			case '%':
				res = new NodeVariant(op1.toLongLong() % op2.toLongLong());
				break;
			default:
				return appendError("Error: invalid operator " + char(op));
		}
		
	} else {
		switch (op) {
			case '+':
				res = new NodeVariant(op1.toLongLong() + op2.toLongLong());
				break;
			case '-':
				res = new NodeVariant(op1.toLongLong() - op2.toLongLong());
				break;
			case '*':
				res = new NodeVariant(op1.toLongLong() * op2.toLongLong());
				break;
			case '/':
				res = new NodeVariant(op1.toLongLong() / op2.toLongLong());
				break;
			case '%':
				res = new NodeVariant(op1.toLongLong() % op2.toLongLong());
				break;
			default:
				return appendError("Error: invalid operator " + char(op));
		}
	}
	return true;
}

}

