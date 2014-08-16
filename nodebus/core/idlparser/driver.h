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

#ifndef IDLPARSER_DRIVER_H
#define IDLPARSER_DRIVER_H

#include <QVariant>
#include <qt4/QtCore/QString>
#include <qt4/QtCore/qstack.h>
#include <qt4/QtCore/QVariant>
#include <qt4/QtCore/QMap>

#define NODE_KEY_TYPE      "T"
#define NODE_KEY_RET_TYPE  "t"
#define NODE_KEY_SYN_NAME  "n"
#define NODE_KEY_DIRECTION "d"
#define NODE_KEY_WRITABLE  "w"
#define NODE_KEY_VALUE     "v"

#define VTYPE_VOID         'v'
#define VTYPE_ANY          'a'
#define VTYPE_BOOLEAN      'b'
#define VTYPE_BYTE         'o'
#define VTYPE_UINT32       'I'
#define VTYPE_INT32        'i'
#define VTYPE_UINT64       'L'
#define VTYPE_INT64        'l'
#define VTYPE_DOUBLE       'd' 
#define VTYPE_STRING       's'
#define VTYPE_BYTEARRAY    'O'
#define VTYPE_DATETIME     't'

#define NTYPE_CONST        'C'
#define NTYPE_ATTR         'A'
#define NTYPE_INTERFACE    'I'
#define NTYPE_METHOD       'M'
#define NTYPE_ENUM         'E'
#define NTYPE_STRUCT       'S'
#define NTYPE_TYPEDEF      'T'

namespace idlparser {

class Parser;
class Scanner;

/**
 * @brief IDL parser driver
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class Driver {
public:
	typedef char (*getc_t)(void *);
	Driver(getc_t getc, void *stream);
	~Driver();
	QVariant parse();
private:
	friend class Parser;
	friend class Scanner;
	QString lastError;
	Scanner &scanner;
	Parser &parser;
	QVariant result;
	QStack<QString> packageStack;
	QVariantMap envGlobal;
	QVariantMap envLocal;
	void blockBegin(const QString& name);
	void blockEnd();
	void addLocal(const QString& name, const QVariant &node);
	bool resolve(const QString& name, QVariant &value);
};

inline void Driver::blockBegin(const QString& name) {
	if (packageStack.isEmpty()) {
		packageStack.push(name);
	} else {
		packageStack.push(packageStack.top() + "::" + name);
	}
}

inline void Driver::blockEnd() {
	QString prefix;
	if (!packageStack.isEmpty()) {
		prefix = packageStack.top() + "::";
	}
	for (auto it = envLocal.begin(); it != envLocal.end(); it++) {
		envGlobal.insert(prefix + it.key(), it.value());
	}
	packageStack.pop();
}

inline void Driver::addLocal(const QString& name, const QVariant &node) {
	envLocal.insert(name, node);
}

inline bool Driver::resolve(const QString& name, QVariant &value) {
	QVariantMap field;
	if (envLocal.contains(name)) {
		field = envLocal[name].toMap();
	} else if (envGlobal.contains(name)) {
		field = envGlobal[name].toMap();
	} else {
		return false;
	}
	if (field.value(NODE_KEY_TYPE).toChar() != NTYPE_CONST) {
		return false;
	}
	value = field.value(NODE_KEY_VALUE);
	return true;
}

}

#endif // IDLPARSER_DRIVER_H
