
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <masterservice.h>
#include <QStringList>

namespace jsonbus {

MasterService::MasterService(int &argc, char **argv)
		: QCoreApplication(argc, argv) {
	m_cliArguments.define("daemonize", 'd', "Launch this service in background");
	m_cliArguments.define("config", 'c', "Set a custom config path", "/etc/jsonbus/jsonbus.conf");
	m_cliArguments.define("help", 'h', "Display this help");
	m_cliArguments.define("setup", 's', "Setup the service");
	m_cliArguments.define("setup-type", '\0', "Setup type (can be \"interactive\" or \"defaults\")", "interactive");
#ifdef WIN32
	m_cliArguments.define("win-cli", '\0', "Switch to cli mode, don't launch a Windows service");
#endif
	m_cliArguments.parse(arguments());
}

MasterService::~MasterService() {
}

void MasterService::launch() {
	
	if (m_cliArguments.isEnabled("setup") || m_cliArguments.isEnabled("setup-default")) {
		bool defaultConf = m_cliArguments.isEnabled("setup-default");
		
	}
}

}
