
#include <typeinfo>
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/jsonparsertask.h>
#include "slaveapplication.h"

SlaveApplication::SlaveApplication(int &argc, char **argv)
	: QCoreApplication(argc, argv) {
}

SlaveApplication::~SlaveApplication() {
}

void SlaveApplication::onAboutToQuit() {
	QThreadPool::globalInstance()->waitForDone();
}

void SlaveApplication::onRunLevelDefineArgs() {
	CliArguments &args = CliArguments::getInstance();
	
	args.define("config",			'c', tr("Set a custom config path"), "");
	args.define("edit-settings",	's', tr("Interactive settings edition"));
	args.define("help",				'h', tr("Display this help"));
}

void SlaveApplication::onRunLevelParseArgs() {
	CliArguments &args = CliArguments::getInstance();
	
	args.parse(arguments());
		
	if (args.isEnabled("help")) {
		args.displayUseInstructions();
		throw ExitApplicationException();
	}
}

void SlaveApplication::onRunLevelSetup() {
	signal(SIGINT, onQuit);
	signal(SIGTERM, onQuit);
	
	JSONParserTask *jsonParserTask = new JSONParserTask(STDIN_FILENO);
	
	connect(jsonParserTask, SIGNAL(terminated()), this, SLOT(quit()));
	connect(jsonParserTask, SIGNAL(dataAvailable(QVariant)), this, SLOT(onDataAvailable(QVariant)));
	connect(this, SIGNAL(aboutToQuit()), jsonParserTask, SLOT(terminate()));
	connect(this, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()), Qt::DirectConnection);
	
	QThreadPool::globalInstance()->start(jsonParserTask);
}

void SlaveApplication::run() {
	try {
		onRunLevelDefineArgs();
		onRunLevelParseArgs();
		onRunLevelSetup();
		cerr << demangle(typeid(*this).name()) << " entring in event loop..." << endl;
		exec();
		cerr << demangle(typeid(*this).name()) << " leaving event loop..." << endl;
	} catch (ExitApplicationException &e) {
		
	} catch (Exception &e) {
		cerr << demangle(typeid(*this).name()) << " aborting start process after throwing an instance of '" << demangle(typeid(e).name()) << "'" << endl;
		if (!e.message().isEmpty())
			cerr << "  what(): " << e.message() << endl;
	}
}

bool SlaveApplication::notify(QObject *rec, QEvent *ev) {
	try {
		return QCoreApplication::notify(rec, ev);
	} catch (Exception &e) {
		cerr << demangle(typeid(*this).name()) << " leaving event loop after throwing an instance of '" << demangle(typeid(e).name()) << "'" << endl;
		if (!e.message().isEmpty())
			cerr << "  what(): " << e.message() << endl;
	}
	quit();
	return false;
}

void SlaveApplication::onQuit(int signum) {
	getInstance().quit();
}
