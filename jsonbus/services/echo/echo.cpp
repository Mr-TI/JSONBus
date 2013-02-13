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

#include <jsonbus/core/common.h>
#include <jsonbus/core/settings.h>
#include <jsonbus/core/logger.h>

#include "echo.h"

jsonbus_declare_plugin(Echo)

Echo::Echo() {
	logFiner() << "Creating echo service...";
}

Echo::~Echo() {
	logFiner() << "Destroying echo service...";
}

void Echo::onInit(Settings &settings) {
	logFiner() << "Initializing echo service...";
	settings.define("fieldName", "The field name", QVariant("name"));
	settings.define("helloMsg", "Hello message", QVariant("Hello"));
};

void Echo::onLoad(Settings &settings) {
	Plugin::onLoad(settings);
	logFiner() << "Loading echo service...";
	m_fieldName = settings.value("fieldName", "name").toString();
	m_helloMsg = settings.value("helloMsg", "Hello").toString();
};

void Echo::onUnload() {
	logFiner() << "Unloading echo service...";
	Plugin::onUnload();
};

void Echo::onRequest(QVariant request) {
	logFiner() << "Request: " << request.toString();
	QVariant result;
	QVariantMap m;
	m["reply"].setValue(m_helloMsg + " " + request.toMap()[m_fieldName].toString());
	m["request"].setValue(request);
	result.setValue(m);
	emit resultAvailable(result);
}

