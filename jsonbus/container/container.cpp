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

#include <typeinfo>
#include <QStringList>

#include <jsonbus/core/common.h>
#include <jsonbus/core/settings.h>
#include <jsonbus/core/jsonserializer.h>

#include "container.h"

#ifdef WIN32
#	define JSONBUS_SERVICEFILE_PREFIX ""
#	define JSONBUS_SERVICEFILE_SUFFIX ".dll"
#else
#	define JSONBUS_SERVICEFILE_PREFIX "lib"
#	define JSONBUS_SERVICEFILE_SUFFIX ".so"
#endif

jsonbus_declare_application(Container)

Container::Container(int &argc, char **argv)
	: SlaveApplication(argc, argv) {
}

Container::~Container() {}

void Container::onRunLevelDefineArgs() {
	SlaveApplication::onRunLevelDefineArgs();
	
	CliArguments &args = CliArguments::getInstance();
	
	args.define("config",		'c', tr("Set the config path"), "");
	
	args.define("bundle-info",	'i', tr("Get bundle info"));
	args.define("bundle-path",	'p', tr("Bundle path"), "");
}

void Container::onRunLevelParseArgs() {
	SlaveApplication::onRunLevelParseArgs();
	
	CliArguments &args = CliArguments::getInstance();
	
	QString bundlePath = args.getValue("bundle-path").toString();
	
	// Checking arguments
	if (bundlePath.isEmpty()) {
		throw ContainerException("Undefinied bundle path");
	}
	
	m_bundle = new Bundle(bundlePath);
}

void Container::onRunLevelSetup()
{
// 	CliArguments &args = CliArguments::getInstance();
// 	
// 	// Load the library containing the plugin
// 	m_libFile = new SharedLib(m_bundlePath);
// 	m_libFile->load();
// 	
// 	// Settings settup
// #ifdef WIN32
// 	Settings settings("OpenIHS.org", "JSONBus::bundles", QSettings::NativeFormat);
// #else
// 	QString confPath = args.getValue("config").toString();
// 	Settings settings(confPath, QSettings::NativeFormat);
// #endif
// 	
// 	// Plugin initialization
// 	m_plugin->onInit(settings);
// 	if (args.isEnabled("edit-settings")) {
// 		settings.setup();
// 		throw ExitApplicationException();
// 	}
// 	
// 	SlaveApplication::onRunLevelSetup();
// 	
// 	// Plugin load
// 	m_plugin->onLoad(settings);
// 	connect(m_plugin.data(), SIGNAL(resultAvailable(QVariant)), this, SLOT(onResultAvailable(QVariant)));
}

void Container::onDataAvailable(QVariant data) {
// 	m_plugin->onRequest(data);
}

void Container::onResultAvailable(QVariant result) {
	m_jsonSerialiser.serialize(result);
	std::cout << std::endl;
}
