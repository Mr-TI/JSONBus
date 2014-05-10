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

/**
 * @brief JSONBus : Settings management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_SETTINGS_H
#define JSONBUS_SETTINGS_H

#include <jsonbus/core/exception.h>

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#ifndef WIN32
#include <dlfcn.h>
#endif
#include <QString>
#include <QVariant>
#include <QSettings>

namespace JSONBus {

jsonbus_declare_exception(SettingsException, Exception);

/**
 * @brief Settings management.
 */
class JSONBUS_EXPORT Settings : public QSettings, public SharedData {
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

	class JSONBUS_EXPORT Element {
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
