
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/settings.h>
#include "container.h"

#ifdef WIN32
#	define JSONBUS_SERVICEFILE_PREFIX ""
#	define JSONBUS_SERVICEFILE_SUFFIX ".dll"
#else
#	define JSONBUS_SERVICEFILE_PREFIX "lib"
#	define JSONBUS_SERVICEFILE_SUFFIX ".so"
#endif

namespace JSONBus {

Container::Container(int &argc, char **argv)
	: QCoreApplication(argc, argv),
	m_pluginFile(NULL), m_plugin(NULL) {
	m_cliArguments.define("service-root",	'd', tr("Plugin root directory (excluding namaspace directory)"), "/usr/lib/jsonbus/services/");
	m_cliArguments.define("service-ns",		'N', tr("Plugin namespace"), "");
	m_cliArguments.define("service-name",	'n', tr("Plugin name"), "");
	m_cliArguments.define("service-path",	'f', tr("Plugin path"), "");
	m_cliArguments.define("config",			'c', tr("Set a custom config path"), "");
	m_cliArguments.define("setup",			's', tr("Setup the service"));
	m_cliArguments.define("help",			'h', tr("Display this help"));
	m_cliArguments.parse(arguments());
}

Container::~Container() {
	if (m_plugin && m_plugin->isLoaded()) {
		m_plugin->onUnload();
	}
	delete m_plugin;
	delete m_pluginFile;
}

void Container::launch() {
	QString serviceRoot = m_cliArguments.getValue("service-root").toString();
	QString serviceName = m_cliArguments.getValue("service-name").toString();
	QString serviceNs = m_cliArguments.getValue("service-ns").toString();
	QString servicePath = m_cliArguments.getValue("service-path").toString();
	
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
	QString confPath = m_cliArguments.getValue("config").toString();
	if (confPath.isEmpty()) {
		confPath = "/etc/jsonbus/services/" + serviceNs + "/" + serviceName + ".conf";
	}
	Settings settings(confPath, QSettings::NativeFormat);
#endif
	
	m_plugin = (*(Plugin*(*)())(m_pluginFile->getSymbol("getSingleton")))();
	
	m_plugin->onInit(settings);
	if (m_cliArguments.isEnabled("setup")) {
		settings.setup();
		return;
	}
	
	m_plugin->onLoad();
	
	connect(m_plugin, SIGNAL(resultAvailable(QVariant)), this, SLOT(onResultAvailable(QVariant)));
	
	exec();
}

void Container::onDataAbailable() {
	
}

void Container::onResultAvailable(QVariant result) {
	
}

}
