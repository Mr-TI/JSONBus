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
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur <contact@openihs.org> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur <contact@openihs.org> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @brief JSONBus : Argument parser management.
 * @file cliarguments.h
 * @author Emeric VERSCHUUR <contact@openihs.org>, (C) 2012
 */

#ifndef JSONBUS_CLIARGUMENTS_H
#define JSONBUS_CLIARGUMENTS_H

#include <jsonbus/core/exception.h>

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#ifndef WIN32
#include <dlfcn.h>
#endif
#include <QString>
#include <QVariant>

namespace JSONBus {

jsonbus_declare_exception(CliArgumentsException, Exception);

/**
 * @brief Dynamic library management.
 */
class JSONBUS_EXPORT CliArguments : public QObject {
public:
	/**
	 * @brief CliArguments constructor.
	 * @param path Dynamic library path.
	 */
	CliArguments();

	/**
	 * @brief CliArguments destructor.
	 */
	~CliArguments();

	/**
	 * @brief Define an argument
	 * @param name argument name (eg: "param-name" for --param-name)
	 * @param shortTag short tag (eg: 'p' for -p), otherwise 0
	 * @param description parameter description
	 * @param hasValue is this parameter have a value
	 * @throw CliArgumentsException on error
	 */
	void define(const QString& name, char shortTag, const QString& description, const QVariant& value = QVariant(false));

	/**
	 * @brief Get an arguement value
	 * @param name argument name
	 * @return QVariant reference to the result
	 * @throw CliArgumentsException on error
	 */
	const QVariant &getValue(const QString& name) const;

	/**
	 * @brief Get an arguement state (enabled/disabled)
	 * @param name argument name
	 * @return bool enabled or not
	 * @throw CliArgumentsException on error
	 */
	bool isEnabled(const QString &name) const;

	/**
	 * @brief Parse the argument list
	 * @param argList the argument list
	 * @throw CliArgumentsException on error
	 */
	void parse(const QStringList &argList);

	/**
	 * @brief Print use instruction
	 */
	void displayUseInstructions();
private:

	class JSONBUS_EXPORT Element {
	public:
		inline Element()
				: name(), shortTag(), value(QVariant(false)), description() {}
		inline Element(const QString &name, char shortTag, const QString &description, const QVariant &value)
				: name(name), shortTag(shortTag), value(value), description(description) {}
		QString name;
		char shortTag;
		QVariant value;
		QString description;
	};

	QMap<QString, Element> m_arguments;
	QMap<char, QString> m_shortTagToName;
	QString m_execName;
};

}

#endif
