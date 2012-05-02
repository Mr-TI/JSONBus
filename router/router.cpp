
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>
#include <router.h>

jsonbus_declare_slave_application(Router)

Router::Router(int &argc, char **argv)
	: SlaveApplication(argc, argv) {
}

Router::~Router() {
}

void Router::onDataAvailable(QVariant data) {
	
}

void Router::onSetup() {
	
}

void Router::launch() {
	CliArguments &args = CliArguments::getInstance();
#ifdef WIN32
	Settings settings("OpenIHS.org", "JSONBus", QSettings::NativeFormat);
#else
	Settings settings(args.getValue("config").toString(), QSettings::NativeFormat);
#endif
	settings.define("router/listen",		tr("Addresses where the router will listen on"),	JSONBUS_DEFAULT_LISTEN_ADDRESSES);
	settings.define("router/plugindir",		tr("Plugin dir paths"), 							JSONBUS_DEFAULT_PLUGIN_DIR_PATH);
	if (args.isEnabled("setup")) {
		settings.setup();
		return;
	}
	
	SlaveApplication::launch();
}
