
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>
#include <container.h>

namespace JSONBus {

Container::Container(int &argc, char **argv)
	: QCoreApplication(argc, argv) {
	m_cliArguments.define("plugin-dir",		'd', tr("Plugin path"), "/usr/lib/jsonbus/plugins/");
	m_cliArguments.define("plugin-name",	'n', tr("Plugin name"), "");
	m_cliArguments.define("plugin-path",	'f', tr("Plugin path"), "");
	m_cliArguments.define("config",			'c', tr("Set a custom config path"), "");
	m_cliArguments.define("setup",			's', tr("Setup the plugin"));
	m_cliArguments.define("help",			'h', tr("Display this help"));
	m_cliArguments.parse(arguments());
}

Container::~Container() {
}

void Container::launch() {
#ifdef WIN32
	Settings settings("OpenIHS.org", "JSONBus::" + m_cliArguments.getValue("plugin-name").toString(), QSettings::NativeFormat);
#else
	Settings settings(m_cliArguments.getValue("config").toString(), QSettings::NativeFormat);
#endif
	if (m_cliArguments.isEnabled("setup")) {
		settings.setup();
		return;
	}
}

}
