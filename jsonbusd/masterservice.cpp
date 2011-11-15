
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <masterservice.h>
#include <QStringList>

namespace jsonbus {

MasterService::MasterService(int &argc, char **argv)
		: QCoreApplication(argc, argv) {
	m_cliArguments.define("daemonize", 'd', tr("Launch this service in background"));
	m_cliArguments.define("config", 'c', tr("Set a custom config path"), "/etc/jsonbus/jsonbus.conf");
	m_cliArguments.define("help", 'h', tr("Display this help"));
	m_cliArguments.define("setup", 's', tr("Setup the service"));
#ifdef WIN32
	m_cliArguments.define("win-cli", '\0', tr("Switch to cli mode, don't launch a Windows service"));
#endif
	m_cliArguments.parse(arguments());
}

MasterService::~MasterService() {
}

void MasterService::launch() {
	
	if (m_cliArguments.isEnabled("setup")) {
		
	}
}

}
