
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <masterservice.h>
#include <QStringList>

namespace jsonbus {
#ifdef WIN32
const bool MasterService::isService = true;
#endif

MasterService::MasterService(int &argc, char **argv)
  : QCoreApplication(argc, argv) {
    m_cliArguments.define("daemonize", 'd', "Launch this service in background");
    m_cliArguments.define("config", 'c', "Set a custom config path", "/etc/jsonbus/jsonbus.conf");
    m_cliArguments.define("help", 'h', "Display this help");
    m_cliArguments.define("win-cli", 0, "Switch to cli mode, don't launch a Windows service");
    m_cliArguments.parse(arguments());
}

MasterService::~MasterService() {
}

void MasterService::launch() {
}

}
