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

#include <cliarguments.h>
#include <common.h>
#include "logger.h"
#include <QStringList>
#include <QRegExp>
#include <QCoreApplication>

namespace NodeBus {

CliArguments CliArguments::instance;

typedef QPair<QString, QString> LineElement;

CliArguments::CliArguments()
{
}

CliArguments::~CliArguments() {
}

const QVariant &CliArguments::getValue(const QString &name) const {
	auto it = m_arguments.find(name);
	if (it == m_arguments.end()) {
		throw CliArgumentsException(tr("The parameter %1 is not defined").arg(name));
	}
	return it.value().value;
}

bool CliArguments::isEnabled(const QString &name) const {
	auto it = m_arguments.find(name);
	if (it == m_arguments.end()) {
		throw CliArgumentsException(tr("The parameter %1 is not defined").arg(name));
	}
	if (it.value().value.type() != QVariant::Bool) {
		throw CliArgumentsException(tr("The parameter %1 is not an boolean value").arg(name));
	}
	return it.value().value.toBool();
}

void CliArguments::define(const QString& name, char shortTag, const QString& description, const QVariant& value) {
	if (m_arguments.find(name) != m_arguments.end()) {
		throw CliArgumentsException(tr("The argument %1 is already defined").arg(name));
	}
	m_arguments.insert(name, Element(name, shortTag, description, value));
	if (shortTag) {
		m_shortTagToName[shortTag] = name;
	}
}

void CliArguments::parse(int argc, const char** argv) {
	QStringList list;
	for (int i = 1; i < argc; i++) {
		list.append(argv[i]);
	}
}

void CliArguments::parse(const QStringList &argList) {
	QString argName = QString::null, argValue;
	QRegExp patternArg("^--([\\w_-]+)(=(.*))?$");
	QRegExp patternShortArg("^-([a-zA-Z]+)(.*)$");
	for (auto it = ++argList.begin(); it != argList.end(); it++) {
		if (patternArg.indexIn(*it) != -1) { // arguments matching --arg-name[=a value] or --arg-name[ a value]
			argName = patternArg.cap(1);
			auto eltIt = m_arguments.find(argName);
			if (eltIt == m_arguments.end()) {
				throw CliArgumentsException(tr("Invalid argument --%1").arg(argName));
			}
			if (eltIt.value().value.type() == QVariant::String) {
				if (!patternArg.cap(2).isEmpty()) {
					eltIt.value().value = patternArg.cap(3);
				} else {
					it++;
					if (it == argList.end()) {
						throw CliArgumentsException(tr("The argument --%1 require a value").arg(argName));
					}
					eltIt.value().value = *it;
				}
			} else {
				eltIt.value().value = true;
			}
		} else if (patternShortArg.indexIn(*it) != -1) { // arguments matching -oneorseveralletters[ value]
			QString sargs = patternShortArg.cap(1);
			for (int i = 0; i < sargs.length(); i++) {
				auto argIt = m_shortTagToName.find(sargs[i].toAscii());
				if (argIt == m_shortTagToName.end()) {
					throw CliArgumentsException(tr("Invalid argument -%1").arg(sargs[i]));
				}
				Element &elt = m_arguments[*argIt];
				if (elt.value.type() == QVariant::String) {
					if (i == (sargs.length() - 1)) {
						if (!patternShortArg.cap(2).isEmpty()) {
							elt.value = patternShortArg.cap(2);
							break;
						} else {
							it++;
							if (it != argList.end()) {
								elt.value = *it;
								break;
							}
						}
					}
					throw CliArgumentsException(tr("The argument -%1 require a value").arg(sargs[i]));
				} else {
					elt.value = true;
				}
			}
		} else {
			m_extraArgs.append(*it);
		}
	}
}

void CliArguments::displayUseInstructions() {
	QList<LineElement> list;
	QString argName;
	QVariant argValue;
	QString execName = QString(QCoreApplication::argv()[0]);
	int maxlen = 0;
	for (auto it = m_arguments.begin(); it != m_arguments.end(); it++) {
		argName = "";
		argValue = it.value().value;
		if (it.value().shortTag) {
			argName = COLOR_VIO " -" + QString(it.value().shortTag) + (argValue.type() == QVariant::String ? " <" + tr("value") + "> " COLOR_YEL "or" : COLOR_YEL " or");
		}
		argName += COLOR_VIO " --" + it.value().name + (argValue.type() == QVariant::String ? "=<" + tr("value") + ">" : "");
		maxlen = qMax(maxlen, argName.length());
		list.append(LineElement(argName, COLOR_WHI + it.value().description + (argValue.type() == QVariant::String ? " (" + tr("value: ") + argValue.toString() + ")" : "")));
	}
	if (!m_helpHeader.isEmpty()) {
		std::cerr << COLOR_GRE << m_helpHeader << std::endl << std::endl;
	}
	std::cerr << COLOR_WHI "  " << tr("Use: ") << COLOR_GRE << execName << COLOR_VIO << 
		tr(" <options>") << COLOR_MAG << " " << m_extraArgsLegend << std::endl << 
		std::endl << COLOR_WHI << "  " << tr("Option list:") << COLOR_RST << std::endl;
	foreach(LineElement pair, list) {
		std::cerr << "    " << pair.first << QString(maxlen - pair.first.length() + 4, ' ') << pair.second << std::endl;
	}
	if (!m_extraArgsDesc.isEmpty()) {
		std::cerr << std::endl << std::endl << m_extraArgsDesc;
	}
}

}
