
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

void Router::onRunLevelDefineArgs()
{
	Application::onRunLevelDefineArgs();
	
	CliArguments &args = CliArguments::getInstance();
	
	args.define("config",			'c', tr("Set a custom config path"), "");
}


void Router::onDataAvailable(QVariant data) {
	
}

void Router::onRunLevelSetup() {
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
	
	SlaveApplication::onRunLevelSetup();
	
// 	QVariantMap data;
// 	data["type"] = "event";
// 	data["uid"] = "";
// 	data["body"] = "";
// 	m_jsonSerialiser.serialize();
}
