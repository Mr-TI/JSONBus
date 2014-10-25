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
 * @brief NodeBus : Settings management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_SETTINGS_H
#define NODEBUS_SETTINGS_H

#include <nodebus/core/exception.h>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#ifndef WIN32
#include <dlfcn.h>
#endif
#include <QString>
#include <QVariant>
#include <QSettings>

namespace NodeBus {

nodebus_declare_exception(SettingsException, Exception);

/**
 * @brief Settings management.
 */
class NODEBUS_EXPORT Settings : public QSettings, public SharedData {
public:
	
	/**
	 * @brief Settings constructor.
	 * @param parent Parent object
	 */
	explicit Settings(QObject* parent = 0);
	
	/**
	 * @brief Settings constructor.
	 * @param fileName Configuration file name
	 * @param format Configuration file format
	 * @param parent Parent object
	 */
	inline Settings(const QString& fileName, Format format, QObject* parent = 0)
		: QSettings(fileName, format, parent){}
	
	/**
	 * @brief Settings constructor.
	 * @param organization Organization name
	 * @param application Application name
	 * @param parent Parent object
	 */
	inline explicit Settings(const QString& organization, const QString& application = QString(), QObject* parent = 0)
		: QSettings(organization, application, parent){}
	
	/**
	 * @brief Settings destructor.
	 */
	~Settings();

	/**
	 * @brief Define an parameter
	 * @param name parameter name
	 * @param description parameter description
	 * @param defaultValue the default value
	 * @throw SettingsException on error
	 */
	void define(const QString& name, const QString& description, const QVariant& defaultValue = QVariant(false));

	/**
	 * @brief Setup parameters
	 */
	void setup();
private:

	class NODEBUS_EXPORT Element {
	public:
		inline Element()
				: value(), description() {}
		inline Element(const QString &description, const QVariant &value)
				: value(value), description(description) {}
		QVariant value;
		QString description;
	};

	QMap<QString, Element> m_parameters;
};

/// @brief Settings shared pointer type
typedef SharedPtr<Settings> SettingsPtr;

}

#endif
