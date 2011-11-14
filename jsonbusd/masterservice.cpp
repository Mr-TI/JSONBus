
#include <jsonbus/core/common.h>
#include <masterservice.h>
#include <QRegExp>

namespace jsonbus {
#ifdef WIN32
const bool MasterService::isService = true;
#endif

MasterService::MasterService(int &argc, char **argv)
  : QCoreApplication(argc, argv) {
}

MasterService::~MasterService() {
}

void MasterService::launch() {
}

}
