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

/**
 * @brief JSONBus : JSONBus application management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_APPLICATION_H
#define JSONBUS_APPLICATION_H

#include <QCoreApplication>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/cliarguments.h>
#include "settings.h"
#include <signal.h>
#include <fcntl.h>

#define jsonbus_declare_application(_class_name_) \
int main(int argc, char **argv) { \
	_class_name_(argc, argv).run(); \
	return 0; \
}

#ifdef WIN32
#define jsonbus_declare_master_service(_class_name_) \
int main(int argc, char **argv) { \
	_class_name_ service(argc, argv); \
	if (args.isEnabled("win32-cli")) { \
		SERVICE_TABLE_ENTRY Table[] = {{"JSONBus Service", Master::runInstance}, {NULL, NULL}}; \
		if (!StartServiceCtrlDispatcher(Table)) { \
			clog << getMessageError(GetLastError()); \
		} \
	} else { \
		service.run(); \
	} \
	return 0; \
}
#else
#define jsonbus_declare_master_service(_class_name_) jsonbus_declare_application(_class_name_)
#endif

namespace JSONBus {

jsonbus_declare_exception(ApplicationException, Exception);
jsonbus_declare_exception(ExitApplicationException, Exception);

/**
 * @brief JSONBus container management.
 */
class Application : public QCoreApplication {
	Q_OBJECT
public:
	/**
	 * @brief Service constructor.
	 */
	Application(int &argc, char **argv);

	/**
	 * @brief Service destructor.
	 */
	~Application();

	/**
	 * @brief Run this application
	 * @throw Exception on error
	 */
	void run();
	
	/**
	 * @brief Get the container instance
	 * @return Application reference
	 */
	inline static Application &getInstance () {
		return *(static_cast<Application*>(instance()));
	}

	/**
	 * @brief Load the container
	 * @throw Exception on error
	 */
	inline static void runInstance() {
		getInstance().run();
	}
	
	/**
	 * @brief bool QCoreApplication::notify(QObject *rec, QEvent *ev) redefinition
	 */
	bool notify(QObject *rec, QEvent *ev);
	
	static void onQuit(int signum);
	
protected:
	/**
	 * @brief Method called during the argument define run level.
	 */
	virtual void onRunLevelDefineArgs();
	
	/**
	 * @brief Method called during the argument parsing run level.
	 */
	virtual void onRunLevelParseArgs();
	
	/**
	 * @brief Method called during the setup run level.
	 */
	virtual void onRunLevelSetup();
	
private slots:
	/**
	 * @brief Called when the application is quitting
	 */
	void onAboutToQuit();
};

}

#endif //JSONBUS_APPLICATION_H
