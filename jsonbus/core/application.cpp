/*
 *   Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
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

namespace JSONBus {

Application::Application(int &argc, char **argv)
	: QCoreApplication(argc, argv) {
}

Application::~Application() {
}

void Application::onAboutToQuit() {
	QThreadPool::globalInstance()->waitForDone();
}

void Application::onRunLevelDefineArgs() {
	CliArguments &args = CliArguments::getInstance();
	
	args.define("edit-settings",	's', tr("Interactive settings edition"));
	args.define("help",				'h', tr("Display this help"));
}

void Application::onRunLevelParseArgs() {
	CliArguments &args = CliArguments::getInstance();
	
	args.parse(arguments());
		
	if (args.isEnabled("help")) {
		args.displayUseInstructions();
		throw ExitApplicationException();
	}
}

void Application::onRunLevelSetup() {
	signal(SIGINT, onQuit);
	signal(SIGTERM, onQuit);
}

void Application::run() {
	try {
		onRunLevelDefineArgs();
		onRunLevelParseArgs();
		onRunLevelSetup();
		connect(this, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()), Qt::DirectConnection);
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

bool Application::notify(QObject *rec, QEvent *ev) {
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

void Application::onQuit(int signum) {
	getInstance().quit();
}

}
