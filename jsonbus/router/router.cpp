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

#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>
#include <router.h>

jsonbus_declare_application(Router)

Router::Router(int &argc, char **argv)
	: SlaveApplication(argc, argv) {
}

Router::~Router() {
}

void Router::onDataAvailable(QVariant data) {
	
}

void Router::onRunLevelSetup()
{
	Application::onRunLevelSetup();
	
	CliArguments &args = CliArguments::getInstance();
	
	args.define("config",			'c', tr("Set a custom config path"), "");
	args.define("edit-settings",	's', tr("Interactive settings edition"));
}

void Router::onRunLevelInit() {
	SlaveApplication::onRunLevelInit();
	CliArguments &args = CliArguments::getInstance();
#ifdef WIN32
	Settings settings("OpenIHS.org", "JSONBus", QSettings::NativeFormat);
#else
	Settings settings(args.getValue("config").toString(), QSettings::NativeFormat);
#endif
	settings.define("router/listen",		tr("Addresses where the router will listen on"),	JSONBUS_DEFAULT_LISTEN_ADDRESSES);
	if (args.isEnabled("edit-settings")) {
		settings.setup();
		throw ExitApplicationException();
	}
}
