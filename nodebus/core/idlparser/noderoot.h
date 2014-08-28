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

#ifndef IDLPARSER_NODEROOT_H
#define IDLPARSER_NODEROOT_H

#include "ltype.h"
#include "node.h"

namespace idlparser {

class NodeRoot: public Node {
private :
	Driver &m_driver;
public:
	NodeRoot(Driver &driver);
	virtual QVariant resolve(const QString symbol, const char *type);
	virtual bool append(NodePtr &pElt);
	virtual QString str();
	QMap<QString, bool> m_fileList;
	QVariantMap m_symTbl;
	QStringList m_errors;
};

inline NodeRoot::NodeRoot(Driver& driver): m_driver(driver) {

}

inline bool NodeRoot::append(NodePtr &pElt) {
	QVariantMap elt = pElt->map();
	QString name = elt[KNODE_SNAME].toString();
	if (m_symTbl.contains(name)) {
		m_driver.appendError("Dupplicate symbol " + name);
		return false;
	}
	m_symTbl.insert(name, elt);
	return true;
}

inline QString NodeRoot::str() {
	static const QString empty;
	return empty; // empty prefix
}

inline QVariant NodeRoot::resolve(const QString symbol, const char *type) {
	static QVariant zero;
	if (!m_symTbl.contains(symbol)) {
		m_driver.appendError("Undefined symbol " + symbol);
		return zero;
	}
	const QVariant &result = m_symTbl[symbol];
	if (result.toMap()[KNODE_TYPE].toString().at(0) != type[0]) {
		m_driver.appendError("Invalid symbol " + symbol);
		return zero;
	}
	return result;
}

}

#endif // IDLPARSER_NODEROOT_H
