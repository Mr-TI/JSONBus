
#include <typeinfo>
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/jsonparserrunnable.h>
#include "slaveapplication.h"

SlaveApplication::SlaveApplication(int &argc, char **argv)
	: QCoreApplication(argc, argv) {
}

SlaveApplication::~SlaveApplication() {
}

void SlaveApplication::onAboutToQuit() {
	QThreadPool::globalInstance()->waitForDone();
}

void SlaveApplication::setup() {
	CliArguments &args = CliArguments::getInstance();
	
	args.define("config",			'c', tr("Set a custom config path"), "");
	args.define("edit-settings",	's', tr("Interactive settings edition"));
	args.define("help",				'h', tr("Display this help"));
	
	onSetup();
	
	args.parse(arguments());
}

void SlaveApplication::launch() {
	JSONParserRunnable *jsonParser = new JSONParserRunnable(STDIN_FILENO);
	connect(this, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()), Qt::DirectConnection);
	connect(this, SIGNAL(aboutToQuit()), jsonParser, SLOT(terminate()));
	connect(jsonParser, SIGNAL(terminated()), this, SLOT(quit()));
	connect(jsonParser, SIGNAL(dataAvailable(QVariant)), this, SLOT(onDataAvailable(QVariant)));
	
	QThreadPool::globalInstance()->start(jsonParser);
	
	exec();
}

bool SlaveApplication::notify(QObject *rec, QEvent *ev) {
	try {
		return QCoreApplication::notify(rec, ev);
	} catch (Exception &e) {
		cerr << ">>> SlaveApplication terminated after throwing an instance of '" << demangle(typeid(e).name()) << "'" << endl;
		cerr << ">>>   what(): " << e.message() << endl;
	} catch (...) {
		qCritical() << ">>> Exception not managed !";
	}
	quit();
	return false;
}

void SlaveApplication::onQuit(int signum) {
	qApp->quit();
}
