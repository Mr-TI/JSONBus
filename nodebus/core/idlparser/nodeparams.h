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

#ifndef IDLPARSER_NODEPARAMS_H
#define IDLPARSER_NODEPARAMS_H

#include "ltype.h"
#include "node.h"

namespace idlparser {

class NodeParams: public Node {
private :
	QHash<QString, bool> m_params;
	QVariantList m_list;
	Driver &m_driver;
public:
	NodeParams(Driver &driver);
	virtual QVariantList &list();
	virtual bool append(SharedPtr<Node> &pElt);
};

inline NodeParams::NodeParams(Driver& driver): m_driver(driver) {
}

inline bool NodeParams::append(SharedPtr<Node> &pElt) {
	QVariantMap &param = pElt->map();
	QString k = param[NODE_KEY_NAME].toString();
	if (m_params.contains(k)) {
		m_driver.appendError("Dupplicated parameter " + k);
		return false;
	}
	m_params.insert(k, true);
	m_list.append(param);
	return true;
}

inline QVariantList& NodeParams::list() {
	return m_list;
}

}

#endif // IDLPARSER_NODEPARAMS_H
