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
	QVariantMap resProps;
	resProps[NODE_KEY_VERSION] = 1;
	resProps[NODE_KEY_MEMBERS] = driver.m_localElts;
	resProps[NODE_KEY_NODE_TYPE] = TYPE_NODE_FRAGMENT;
	return resProps;
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
	switch (var[NODE_KEY_DATA_TYPE].toString()[0].toAscii()) {
		case TYPE_VAL_VOID[0]:
			return appendError("Error: invalid variable type void");
		case TYPE_VAL_ANY[0]:
			opDataType = QVariant::Invalid;
			break;
		case TYPE_VAL_BOOLEAN[0]:
			opDataType = QVariant::Bool;
			break;
		case TYPE_VAL_BYTE[0]:
			opDataType = QVariant::Char;
			break;
		case TYPE_VAL_UINT32[0]:
			opDataType = QVariant::UInt;
			break;
		case TYPE_VAL_INT32[0]:
			opDataType = QVariant::Int;
			break;
		case TYPE_VAL_UINT64[0]:
			opDataType = QVariant::ULongLong;
			break;
		case TYPE_VAL_INT64[0]:
			opDataType = QVariant::LongLong;
			break;
		case TYPE_VAL_DOUBLE[0]:
			opDataType = QVariant::Double;
			break;
		case TYPE_VAL_STRING[0]:
			opDataType = QVariant::String;
			break;
		case TYPE_VAL_BYTEARRAY[0]:
			opDataType = QVariant::BitArray;
			break;
		case TYPE_VAL_DATETIME[0]:
			opDataType = QVariant::DateTime;
			break;
		default:
			return appendError("Error: invalid variable type");
			break;
	}
	if (!res.convert(opDataType)) {
		return appendError("Error: the result cannot be converted to " + type2str(opDataType));
	}
	var.insert(NODE_KEY_VALUE, res);
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

QVariantMap& NodeIntf::map() {
	if (!m_finalized) {
		m_infos.insert(NODE_KEY_MEMBERS, m_members);
		m_infos.insert(NODE_KEY_NODE_TYPE, TYPE_NODE_INTERFACE);
		QMap<QString, QString> symMap;
		QString nameI = m_infos[NODE_KEY_NAME].toString();
		for (auto member : m_members) {
			QString memberName = member.toMap()[NODE_KEY_NAME].toString();
			symMap[memberName] = nameI;
		}
		for (auto it = m_parents.begin(); it != m_parents.end(); it++) {
			QString nameP = it.key();
			auto membersP = it.value()->m_members;
			for (auto member : membersP) {
				QString memberName = member.toMap()[NODE_KEY_NAME].toString();
				if (symMap.contains(memberName)) {
					m_driver.appendError("Interface " + nameI + ": conflict between the two symbols " +
					symMap[memberName] + NAMESPACE_SEP + memberName + " and " + nameP + NAMESPACE_SEP + memberName);
					continue;
				}
				symMap[memberName] = nameP;
			}
		}
		m_finalized = true;
	}
	return m_infos;
}

}

