
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>
#include "master.h"

jsonbus_declare_master_service(Master)

Master::Master(int &argc, char **argv)
	: Application(argc, argv) {
}

Master::~Master() {
}

void Master::onRunLevelDefineArgs() {
	Application::onRunLevelDefineArgs();
	
	CliArguments &args = CliArguments::getInstance();
	args.define("daemonize",	'd', tr("Launch this service in background"));
	args.define("config",		'c', tr("Set a custom config path"), "/etc/jsonbus/jsonbus.conf");
#ifdef WIN32
	args.define("win-cli",		'\0', tr("Switch to cli mode, don't launch a Windows service"));
#endif
}

void Master::onRunLevelSetup() {
	Application::onRunLevelSetup();
	
	CliArguments &args = CliArguments::getInstance();
#ifdef WIN32
	Settings settings("OpenIHS.org", "JSONBus", QSettings::NativeFormat);
#else
	Settings settings(args.getValue("config").toString(), QSettings::NativeFormat);
#endif
	settings.define("master/pidfile",	tr("Path of the file where the service PID will be written in"),
					JSONBUS_DEFAULT_PIDFILE);
	settings.define("master/plugindir",	tr("Plugin dir paths"), 
					JSONBUS_DEFAULT_PLUGIN_DIR_PATH);
	if (args.isEnabled("edit-settings")) {
		settings.setup();
		throw ExitApplicationException();
	}
}
