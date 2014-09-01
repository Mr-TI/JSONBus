/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
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
#include <nodebus/core/common.h>
#include "application.h"
#include "logger.h"

static void sigsegv_signal_handler(int sig) {

	fprintf(stderr, "\n##### SEGMENTATION FAULT #####\n\n");

	void *trace[32];
	size_t size, i;
	char **strings;

	size    = backtrace( trace, 32 );
	strings = backtrace_symbols( trace, size );

	fprintf( stderr, "\nBACKTRACE:\n\n" );

	for( i = 0; i < size; i++ ){
		fprintf( stderr, "  %s\n", strings[i] );
	}
	
	free(strings);

	fprintf(stderr, "\n##############################\n\n");

	exit(-1);
}

static void sigpipe_signal_handler(int sig) {
	fprintf(stderr, "### SIGPIPE ###\n");
}

static void sigusr1_signal_handler(int sig) {
}

static void sigusr2_signal_handler(int sig) {
	void *trace[32];
	size_t size, i;
	char **strings;

	size    = backtrace( trace, 32 );
	strings = backtrace_symbols( trace, size );

	fprintf( stderr, "\nBACKTRACE:\n\n" );

	for( i = 0; i < size; i++ ){
		fprintf( stderr, "  %s\n", strings[i] );
	}
	
	free(strings);
	
	fprintf(stderr, "\n##############################\n\n");
}

static void sighup_signal_handler(int sig) {
	fprintf(stderr, "### SIGHUP ###\n");
}

namespace NodeBus {

Application::Application(int &argc, char **argv)
	: QCoreApplication(argc, argv) {
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = sigsegv_signal_handler;
	sigaction( SIGSEGV, &act, NULL);
	act.sa_handler = sigpipe_signal_handler;
	sigaction( SIGPIPE, &act, NULL);
	act.sa_handler = sighup_signal_handler;
	sigaction( SIGHUP, &act, NULL);
	act.sa_handler = sigusr1_signal_handler;
	sigaction( SIGUSR1, &act, NULL);
	act.sa_handler = sigusr2_signal_handler;
	sigaction( SIGUSR2, &act, NULL);
}

Application::~Application() {
}

void Application::onAboutToQuit() {
	QThreadPool::globalInstance()->waitForDone();
}

void Application::onExec() {
	onStart();
	signal(SIGINT, onQuit);
	signal(SIGTERM, onQuit);
	connect(this, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()), Qt::DirectConnection);
	logInfo() << __demangle(typeid(*this).name()) << " started.";
	exec();
	logFiner() << __demangle(typeid(*this).name()) << " stopping...";
	onStop();;
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
		onExec();
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
