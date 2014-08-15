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
	void packagePush(const QVariant& value);
	void packagePop();
	QString absoluteName(const QVariant& value);
};

inline void Driver::packagePush(const QVariant& value) {
	if (packageStack.isEmpty()) {
		packageStack.push(value.toString());
	} else {
		packageStack.push(packageStack.top() + "::" + value.toString());
	}
}

inline void Driver::packagePop() {
	packageStack.pop();
}

inline QString Driver::absoluteName(const QVariant& value) {
	if (packageStack.isEmpty()) {
		return value.toString();
	} else {
		return packageStack.top() + "::" + value.toString();
	}
}

}

#endif // IDLPARSER_DRIVER_H
