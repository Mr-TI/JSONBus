/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
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

#include <QStringList>
#include <nodebus/core/common.h>
#include <nodebus/core/cliarguments.h>
#include <nodebus/core/settings.h>
#include <nodebus/core/logger.h>
#include "master.h"

nodebus_declare_master_service(Master)

Master::Master(int &argc, char **argv)
	: Application(argc, argv) {
}

Master::~Master() {
}

void Master::onInit() {
	CliArguments &args = CliArguments::getInstance();
	args.define("daemonize",		'd', tr("Launch this service in background"));
	args.define("edit-settings",	's', tr("Interactive settings edition"));
	args.define("config",			'c', tr("Set a custom config path"), "/etc/nodebus/nodebus.conf");
#ifdef WIN32
	args.define("win-cli",			'\0', tr("Switch to cli mode, don't launch a Windows service"));
#endif
}

void Master::onStart() {
	CliArguments &args = CliArguments::getInstance();
#ifdef WIN32
	m_settings = new Settings("mbedsys.org", "NodeBus", QSettings::NativeFormat);
#else
	m_settings = new Settings(args.getValue("config").toString(), QSettings::NativeFormat);
#endif
	m_settings->define("master/pidfile",	tr("Path of the file where the service PID will be written in"),
					NODEBUS_DEFAULT_PIDFILE);
	m_settings->define("master/bundle-rootpath",	tr("Bundle root directory path"), 
					NODEBUS_DEFAULT_PLUGIN_DIR_PATH);
	m_settings->define("master/registry-service-name",	tr("Registry service name"), 
					NODEBUS_DEFAULT_REGISTRY_SERVICE_NAME);
	if (args.isEnabled("edit-settings")) {
		m_settings->setup();
		throw ExitApplicationException();
	}
	QString path = m_settings->value("master/bundle-rootpath").toString();
	QDirIterator it(path, QStringList("*.so"), QDir::Files, QDirIterator::Subdirectories);
	logFiner() << "Search for bundles in the directory " << path;
	while (it.hasNext()) {
		QString file = it.next();
		logFinest() << "Found file " << file;
		try {
			BundlePtr bundle = new Bundle(file);
			m_bundles[bundle->property("Bundle-SymbolicName").toString()] = bundle;
			logFine() << "Found bundle " << bundle->property("Bundle-Name") << " (" << bundle->property("Bundle-SymbolicName") << ')';
		} catch (Exception &e) {
			logWarn() << "Invalid bundle file " << file << " (" << e.message() << ')';
		}
	}
	if (m_bundles.isEmpty()) {
		throw ApplicationException("No valid bundle found in the directory " + path);
	}
}

