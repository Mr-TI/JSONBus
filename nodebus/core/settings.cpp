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

#include <common.h>
#include <settings.h>
#include "logger.h"

namespace NodeBus {

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
	std::cerr << std::endl << "\t\t\033[1;33m" << tr(">> Configuration setup <<") << "\033[0m" << std::endl << std::endl;
	QString newVal;
	std::string buff;
	for (auto it = m_parameters.begin(); it != m_parameters.end(); it++) {
		std::cerr << "\t\033[1;36m" << it.value().description  << " \033[1;37m[\033[1;35m" << value(it.key()).toString() << "\033[1;37m]: \033[1;32m";
		getline (std::cin, buff);
		newVal = QString::fromStdString(buff);
		if (!newVal.isEmpty()) {
			setValue(it.key(), newVal);
		}
	}
	std::cerr << "\033[0m" << std::endl;
	sync();
}

}
