
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>
#include <master.h>

namespace JSONBus {

Master::Master(int &argc, char **argv)
	: QCoreApplication(argc, argv) {
	CliArguments &args = CliArguments::getInstance();
	args.define("daemonize",	'd', tr("Launch this service in background"));
	args.define("config",		'c', tr("Set a custom config path"), "/etc/jsonbus/jsonbus.conf");
	args.define("help",		'h', tr("Display this help"));
	args.define("setup",		's', tr("Setup the service"));
#ifdef WIN32
	args.define("win-cli",	'\0', tr("Switch to cli mode, don't launch a Windows service"));
#endif
	args.parse(arguments());
}

Master::~Master() {
}

void Master::launch() {
	CliArguments &args = CliArguments::getInstance();
#ifdef WIN32
	Settings settings("OpenIHS.org", "JSONBus", QSettings::NativeFormat);
#else
	Settings settings(args.getValue("config").toString(), QSettings::NativeFormat);
#endif
	settings.define("master/pidfile",		tr("Path of the file where the service PID will be written in"),	JSONBUS_DEFAULT_PIDFILE);
	settings.define("router/listen",		tr("Addresses where the router will listen on"), 					JSONBUS_DEFAULT_LISTEN_ADDRESSES);
	settings.define("router/plugindir",		tr("Plugin dir paths"), 											JSONBUS_DEFAULT_PLUGIN_DIR_PATH);
	if (args.isEnabled("setup")) {
		settings.setup();
		return;
	}
}

}
