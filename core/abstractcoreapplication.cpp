
#include <typeinfo>
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/settings.h>
#include <jsonbus/core/jsonparserrunnable.h>
#include <jsonbus/core/jsonserializer.h>
#include "abstractcoreapplication.h"
#include <fcntl.h>

#ifdef WIN32
#	define JSONBUS_SERVICEFILE_PREFIX ""
#	define JSONBUS_SERVICEFILE_SUFFIX ".dll"
#else
#	define JSONBUS_SERVICEFILE_PREFIX "lib"
#	define JSONBUS_SERVICEFILE_SUFFIX ".so"
#endif

AbstractCoreApplication::AbstractCoreApplication(int &argc, char **argv)
	: QCoreApplication(argc, argv),
	m_pluginFile(NULL), m_plugin(NULL) {
	
}

AbstractCoreApplication::~AbstractCoreApplication() {
	emit terminated();
	QThreadPool::globalInstance()->waitForDone();
	if (m_plugin && m_plugin->isLoaded()) {
		m_plugin->onUnload();
	}
	delete m_plugin;
	delete m_pluginFile;
}

void AbstractCoreApplication::setupCliArguments(CliArguments& args) {
	m_cliArguments.define("config",			'c', tr("Set a custom config path"), "");
	m_cliArguments.define("setup",			's', tr("Setup the service"));
	m_cliArguments.define("help",			'h', tr("Display this help"));
	
	m_cliArguments.parse(arguments());
}

void AbstractCoreApplication::launch() {
	QString serviceRoot = m_cliArguments.getValue("service-root").toString();
	QString serviceName = m_cliArguments.getValue("service-name").toString();
	QString serviceNs = m_cliArguments.getValue("service-ns").toString();
	QString servicePath = m_cliArguments.getValue("service-path").toString();
	
	if (serviceNs.isEmpty()) {
		throw AbstractCoreApplicationException("Undefinied service namespace");
	}
	if (serviceName.isEmpty()) {
		throw AbstractCoreApplicationException("Undefinied service name");
	}
	
	if (servicePath.isEmpty()) {
		if (serviceRoot.isEmpty()) {
			throw AbstractCoreApplicationException("Undefinied root service directory");
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
	
	m_plugin->onLoad(settings);
	
	JSONParserRunnable *jsonParser = new JSONParserRunnable(STDIN_FILENO);
	connect(this, SIGNAL(terminated()), jsonParser, SLOT(terminate()));
	connect(jsonParser, SIGNAL(terminated()), this, SLOT(quit()));
	connect(m_plugin, SIGNAL(resultAvailable(QVariant)), this, SLOT(onResultAvailable(QVariant)));
	connect(jsonParser, SIGNAL(dataAvailable(QVariant)), this, SLOT(onDataAvailable(QVariant)));
	
	QThreadPool::globalInstance()->start(jsonParser);
	
	exec();
}

void AbstractCoreApplication::onDataAvailable(QVariant data) {
	m_plugin->onRequest(data);
}

void AbstractCoreApplication::onResultAvailable(QVariant result) {
	m_jsonSerialiser.serialize(result);
	cout << endl;
}

bool AbstractCoreApplication::notify(QObject *rec, QEvent *ev) {
	try {
		return QCoreApplication::notify(rec, ev);
	} catch (Exception &e) {
		cerr << ">>> AbstractCoreApplication terminated after throwing an instance of '" << demangle(typeid(e).name()) << "'" << endl;
		cerr << ">>>   what(): " << e.message() << endl;
	} catch (...) {
		qCritical() << ">>> Exception not managed !";
	}
	quit();
	return false;
}
