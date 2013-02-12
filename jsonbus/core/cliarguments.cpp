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

#include <cliarguments.h>
#include <common.h>
#include "logger.h"
#include <QStringList>
#include <QRegExp>
#include <QCoreApplication>

namespace JSONBus {

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
			throw CliArgumentsException(tr("Unexpected argument %1").arg(*it));
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
			argName = " -" + QString(it.value().shortTag) + (argValue.type() == QVariant::String ? " <" + tr("value") + "> or" : " or");
		}
		argName += " --" + it.value().name + (argValue.type() == QVariant::String ? "=<" + tr("value") + ">" : "");
		maxlen = qMax(maxlen, argName.length());
		list.append(LineElement(argName, it.value().description + (argValue.type() == QVariant::String ? " (" + tr("value: ") + argValue.toString() + ")" : "")));
	}
	logInfo() << tr("Use: ") << execName << tr(" <options>") << endl << endl << tr("Option list:");
	foreach(LineElement pair, list) {
		logInfo() << pair.first << QString(maxlen - pair.first.length() + 4, ' ') << pair.second;
	}
}

}
