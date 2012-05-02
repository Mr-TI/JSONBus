
#include <typeinfo>
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/settings.h>
#include "container.h"
#include <jsonbus/core/jsonparserrunnable.h>
#include <fcntl.h>
#include <jsonbus/core/jsonserializer.h>

#ifdef WIN32
#	define JSONBUS_SERVICEFILE_PREFIX ""
#	define JSONBUS_SERVICEFILE_SUFFIX ".dll"
#else
#	define JSONBUS_SERVICEFILE_PREFIX "lib"
#	define JSONBUS_SERVICEFILE_SUFFIX ".so"
#endif

// jsonbus_declare_slave_application(Container)

int main(int argc, char **argv) {
	Container app(argc, argv);
	app.setup();
	CliArguments &args = CliArguments::getInstance();
	if (args.isEnabled("help")) {
		args.displayUseInstructions();
		return 0;
	}
	signal(SIGINT, SlaveApplication::onQuit);
	signal(SIGTERM, SlaveApplication::onQuit);
	app.launch();
	return 0;
}

Container::Container(int &argc, char **argv)
	: SlaveApplication(argc, argv),
	m_pluginFile(NULL), m_plugin(NULL) {
}

Container::~Container() {
	if (m_plugin && m_plugin->isLoaded()) {
		m_plugin->onUnload();
	}
	delete m_plugin;
	delete m_pluginFile;
}

void Container::onSetup() {
	CliArguments &args = CliArguments::getInstance();
	args.define("service-root",	'd', tr("Plugin root directory (excluding namaspace directory)"), "/usr/lib/jsonbus/services/");
	args.define("service-ns",		'N', tr("Plugin namespace"), "");
	args.define("service-name",	'n', tr("Plugin name"), "");
	args.define("service-path",	'f', tr("Plugin path"), "");
}

void Container::launch() {
	CliArguments &args = CliArguments::getInstance();
	QString serviceRoot = args.getValue("service-root").toString();
	QString serviceName = args.getValue("service-name").toString();
	QString serviceNs = args.getValue("service-ns").toString();
	QString servicePath = args.getValue("service-path").toString();
	
	if (serviceNs.isEmpty()) {
		throw ContainerException("Undefinied service namespace");
	}
	if (serviceName.isEmpty()) {
		throw ContainerException("Undefinied service name");
	}
	
	if (servicePath.isEmpty()) {
		if (serviceRoot.isEmpty()) {
			throw ContainerException("Undefinied root service directory");
		}
		servicePath = serviceRoot + "/" + serviceNs + "/" JSONBUS_SERVICEFILE_PREFIX + serviceName + JSONBUS_SERVICEFILE_SUFFIX;
	}
	
	m_pluginFile = new SharedLib(servicePath);
	m_pluginFile->load();
	
#ifdef WIN32
	Settings settings("OpenIHS.org", "JSONBus::" + serviceNs + "." + serviceName, QSettings::NativeFormat);
#else
	QString confPath = args.getValue("config").toString();
	if (confPath.isEmpty()) {
		confPath = "/etc/jsonbus/services/" + serviceNs + "/" + serviceName + ".conf";
	}
	Settings settings(confPath, QSettings::NativeFormat);
#endif
	
	m_plugin = (*(Plugin*(*)())(m_pluginFile->getSymbol("getSingleton")))();
	
	m_plugin->onInit(settings);
	if (args.isEnabled("edit-settings")) {
		settings.setup();
		return;
	}
	
	m_plugin->onLoad(settings);
	
	connect(m_plugin, SIGNAL(resultAvailable(QVariant)), this, SLOT(onResultAvailable(QVariant)));
	SlaveApplication::launch();
}

void Container::onDataAvailable(QVariant data) {
	m_plugin->onRequest(data);
}

void Container::onResultAvailable(QVariant result) {
	m_jsonSerialiser.serialize(result);
	cout << endl;
}
