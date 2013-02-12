/*
 *   Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
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

#include <common.h>
#include <settings.h>

namespace JSONBus {

Settings::~Settings() {
}

void Settings::define(const QString& name, const QString& description, const QVariant& defaultValue) {
	if (m_parameters.find(name) != m_parameters.end()) {
		throw SettingsException(tr("The parameter %1 is already definied").arg(name));
	}
	m_parameters.insert(name, Element(description, defaultValue));
	if (!contains(name)) {
		setValue(name, defaultValue);
	}
}

void Settings::setup() {
	cout << tr("Configuration setup") << endl;
	QString newVal;
	string buff;
	for (auto it = m_parameters.begin(); it != m_parameters.end(); it++) {
		cout << it.value().description  << " [" << value(it.key()).toString() << "]: ";
		getline (cin, buff);
		newVal = QString::fromStdString(buff);
		if (!newVal.isEmpty()) {
			setValue(it.key(), newVal);
		}
	}
	sync();
}

}
