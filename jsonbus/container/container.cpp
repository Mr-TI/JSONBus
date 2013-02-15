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

Container::~Container() {
	if (m_plugin != null && m_plugin->isLoaded()) {
		m_plugin->onUnload();
	}
}

void Container::onRunLevelDefineArgs() {
	SlaveApplication::onRunLevelDefineArgs();
	
	CliArguments &args = CliArguments::getInstance();
	
	args.define("config",		'c', tr("Set a custom config path"), "");
	
	args.define("service-root",	'd', tr("Plugin root directory (excluding namaspace directory)"), "/usr/lib/jsonbus/services/");
	args.define("service-ns",	'N', tr("Plugin namespace"), "");
	args.define("service-name",	'n', tr("Plugin name"), "");
	args.define("service-path",	'f', tr("Plugin path"), "");
}

void Container::onRunLevelParseArgs() {
	SlaveApplication::onRunLevelParseArgs();
	
	CliArguments &args = CliArguments::getInstance();
	
	m_serviceRoot = args.getValue("service-root").toString();
	m_serviceName = args.getValue("service-name").toString();
	m_serviceNs = args.getValue("service-ns").toString();
	m_servicePath = args.getValue("service-path").toString();
	
	// Checking arguments
	if (m_serviceNs.isEmpty()) {
		throw ContainerException("Undefinied service namespace");
	}
	if (m_serviceName.isEmpty()) {
		throw ContainerException("Undefinied service name");
	}
	
	if (m_servicePath.isEmpty()) {
		if (m_serviceRoot.isEmpty()) {
			throw ContainerException("Undefinied root service directory");
		}
		m_servicePath = m_serviceRoot + "/" + m_serviceNs + "/" JSONBUS_SERVICEFILE_PREFIX + m_serviceName + JSONBUS_SERVICEFILE_SUFFIX;
	}
}

void Container::onRunLevelSetup()
{
	CliArguments &args = CliArguments::getInstance();
	
	// Load the library containing the plugin
	m_pluginFile = new SharedLib(m_servicePath);
	m_pluginFile->load();
	
	// Settings settup
#ifdef WIN32
	Settings settings("OpenIHS.org", "JSONBus::" + serviceNs + "." + serviceName, QSettings::NativeFormat);
#else
	QString confPath = args.getValue("config").toString();
	if (confPath.isEmpty()) {
		confPath = "/etc/jsonbus/services/" + m_serviceNs + "/" + m_serviceName + ".conf";
	}
	Settings settings(confPath, QSettings::NativeFormat);
#endif
	
	// Get the plugin instance
	m_plugin = (*(PluginPtr(*)())(m_pluginFile->getSymbol("getSingleton")))();
	
	// Plugin initialization
	m_plugin->onInit(settings);
	if (args.isEnabled("edit-settings")) {
		settings.setup();
		throw ExitApplicationException();
	}
	
	SlaveApplication::onRunLevelSetup();
	
	// Plugin load
	m_plugin->onLoad(settings);
	connect(m_plugin.data(), SIGNAL(resultAvailable(QVariant)), this, SLOT(onResultAvailable(QVariant)));
}

void Container::onDataAvailable(QVariant data) {
	m_plugin->onRequest(data);
}

void Container::onResultAvailable(QVariant result) {
	m_jsonSerialiser.serialize(result);
	std::cout << std::endl;
}
