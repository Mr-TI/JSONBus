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

/**
 * @brief NodeBus : Argument parser management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_CLIARGUMENTS_H
#define NODEBUS_CLIARGUMENTS_H

#include <nodebus/core/exception.h>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#ifndef WIN32
#include <dlfcn.h>
#endif
#include <QString>
#include <QVariant>

namespace NodeBus {

nodebus_declare_exception(CliArgumentsException, Exception);

/**
 * @brief Dynamic library management.
 */
class NODEBUS_EXPORT CliArguments : public QObject {
public:

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
	 * @brief Parse the argument list
	 * @param argc argument count
	 * @param argv argument table
	 * @throw CliArgumentsException on error
	 */
	void parse(int argc, const char **argv);

	/**
	 * @brief Get the extra argrgument list
	 * @return List QString
	 */
	const QStringList &extraArgs();

	/**
	 * @brief Print use instruction
	 */
	void displayUseInstructions();
	
	void setHelpHeader(const QString &header);
	
	void setExtraArgsLegend(const QString &legend);
	
	void setExtraArgsDesc(const QString &desc);
	
	inline static CliArguments &getInstance() { return instance; };
private:
	/**
	 * @brief CliArguments constructor.
	 * @param path Dynamic library path.
	 */
	CliArguments();
	
	static CliArguments instance;

	class NODEBUS_EXPORT Element {
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
	QStringList m_extraArgs;
	QString m_helpHeader;
	QString m_extraArgsLegend;
	QString m_extraArgsDesc;
};

inline const QStringList& CliArguments::extraArgs() {
	return m_extraArgs;
}

inline void NodeBus::CliArguments::setExtraArgsLegend(const QString &legend) {
	m_extraArgsLegend = legend;
}

inline void CliArguments::setExtraArgsDesc(const QString &desc) {
	m_extraArgsDesc = desc;
}

inline void CliArguments::setHelpHeader(const QString& header) {
	m_helpHeader = header;
}

}

#endif
