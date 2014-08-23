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

#ifndef IDLPARSER_NODEMAP_H
#define IDLPARSER_NODEMAP_H

#include "ltype.h"
#include "node.h"

namespace idlparser {

class NodeMap: public Node {
private:
	QVariantMap m_map;
public:
	virtual QVariantMap &map();
	virtual Node *insert(const QString &key, const QVariant &value);
};

inline QVariantMap& NodeMap::map() {
	return m_map;
}

inline Node * NodeMap::insert(const QString &key, const QVariant &value) {
	m_map.insert(key, value);
	return this;
}

}

#endif // IDLLTYPE_H
