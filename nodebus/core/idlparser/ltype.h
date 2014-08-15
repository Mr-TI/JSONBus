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

#ifndef IDLLTYPE_H
#define IDLLTYPE_H

#define YYSTYPE idlparser::LType

#include <QVariant>
#include <QString>

namespace idlparser {
	
class Driver;

class LType {
public:
	LType();
	~LType();
	QVariant *var;
	QString *str;
};

inline LType::LType(): var(NULL), str(NULL) {
}

inline LType::~LType() {
	delete var;
	delete str;
}


}

#endif // IDLLTYPE_H
