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
 * @brief JSONBus : Settings management.
 * @file settings.h
 * @author Emeric VERSCHUUR <contact@openihs.org>, (C) 2012
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
class JSONBUS_EXPORT Settings : public QSettings {
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

}

#endif
