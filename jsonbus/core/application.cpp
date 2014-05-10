/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@openihs.org>
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *		   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <typeinfo>
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/jsonparsertask.h>
#include "application.h"
#include "logger.h"

namespace JSONBus {

Application::Application(int &argc, char **argv)
	: QCoreApplication(argc, argv) {
}

Application::~Application() {
}

void Application::onAboutToQuit() {
	QThreadPool::globalInstance()->waitForDone();
}

void Application::run() {
	CliArguments &args = CliArguments::getInstance();
	try {
		args.define("help", 'h', tr("Display this help"));
		onInit();
		args.parse(arguments());
		if (args.isEnabled("help")) {
			args.displayUseInstructions();
			throw ExitApplicationException();
		}
		onStart();
		signal(SIGINT, onQuit);
		signal(SIGTERM, onQuit);
		connect(this, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()), Qt::DirectConnection);
		logInfo() << __demangle(typeid(*this).name()) << " started.";
		exec();
		logFiner() << __demangle(typeid(*this).name()) << " stopping...";
	} catch (ExitApplicationException &e) {
		
	} catch (Exception &e) {
		logCrit() << __demangle(typeid(*this).name()) << " aborting start process after throwing an " << e;
	}
}

bool Application::notify(QObject *rec, QEvent *ev) {
	try {
		return QCoreApplication::notify(rec, ev);
	} catch (Exception &e) {
		logConf() << __demangle(typeid(*this).name()) << " leaving event loop after throwing an " << e;
	}
	quit();
	return false;
}

void Application::onQuit(int signum) {
	getInstance().quit();
}

}
