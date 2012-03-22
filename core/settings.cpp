/*
    Copyright (c) 2012, Emeric Verschuur <contact@openihs.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the OpenIHS.org nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
