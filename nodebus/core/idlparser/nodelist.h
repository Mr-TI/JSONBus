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

#ifndef IDLPARSER_NODELIST_H
#define IDLPARSER_NODELIST_H

#include "ltype.h"
#include "node.h"

namespace idlparser {

class NodeList: public Node {
private:
	QVariantList m_list;
public:
	virtual QVariantList &list();
	virtual Node *append(const QVariant &value);
};

inline QVariantList& NodeList::list() {
	return m_list;
}

inline Node *NodeList::append(const QVariant &value) {
	m_list.append(value);
	return this;
}

}

#endif // IDLPARSER_NODELIST_H
