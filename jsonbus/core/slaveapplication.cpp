
#include <unistd.h>
#include <typeinfo>
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/jsonparsertask.h>
#include "slaveapplication.h"

namespace JSONBus {

SlaveApplication::SlaveApplication(int &argc, char **argv)
	: Application(argc, argv) {
}

SlaveApplication::~SlaveApplication() {
}

void SlaveApplication::onRunLevelSetup() {
	Application::onRunLevelSetup();
	
	JSONParserTask *jsonParserTask = new JSONParserTask(STDIN_FILENO);
	
	connect(jsonParserTask, SIGNAL(terminated()), this, SLOT(quit()));
	connect(jsonParserTask, SIGNAL(dataAvailable(QVariant)), this, SLOT(onDataAvailable(QVariant)));
	connect(this, SIGNAL(aboutToQuit()), jsonParserTask, SLOT(terminate()));
	
	QThreadPool::globalInstance()->start(jsonParserTask);
}

}
