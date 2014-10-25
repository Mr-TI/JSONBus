/*
 * Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * @brief NodeBus : NodeBus application management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */

#ifndef NODEBUS_APPLICATION_H
#define NODEBUS_APPLICATION_H

#include <QCoreApplication>
#include <nodebus/core/exception.h>
#include <nodebus/core/cliarguments.h>
#include <nodebus/core/settings.h>
#include <signal.h>
#include <fcntl.h>

#define nodebus_declare_application(_class_name_) \
int main(int argc, char **argv) { \
	return _class_name_(argc, argv).run(); \
}

#ifdef WIN32
#define nodebus_declare_master_service(_class_name_) \
int main(int argc, char **argv) { \
	_class_name_ service(argc, argv); \
	if (args.isEnabled("win32-cli")) { \
		SERVICE_TABLE_ENTRY Table[] = {{"NodeBus Service", Master::runInstance}, {NULL, NULL}}; \
		if (!StartServiceCtrlDispatcher(Table)) { \
			clog << getMessageError(GetLastError()); \
		} \
	} else { \
		return service.run(); \
	} \
	return 1; \
}
#else
#define nodebus_declare_master_service(_class_name_) nodebus_declare_application(_class_name_)
#endif

namespace NodeBus {

nodebus_declare_exception(ApplicationException, Exception);
nodebus_declare_exception(ExitApplicationException, Exception);

/**
 * @brief NodeBus container management.
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
	int run();
	
	/**
	 * @brief Get the container instance
	 * @return Application reference
	 */
	static Application &getInstance ();

	/**
	 * @brief Load the container
	 * @throw Exception on error
	 */
	static void runInstance();
	
	/**
	 * @brief bool QCoreApplication::notify(QObject *rec, QEvent *ev) redefinition
	 */
	bool notify(QObject *rec, QEvent *ev);
	
	static void onQuit(int signum);
	
protected:
	
	/**
	 * @brief Method called during inititialization process
	 */
	virtual void onInit() = 0;
	
	/**
	 * @brief Method called during start process
	 */
	virtual void onStart();
	
	/**
	 * @brief Method called during exec process
	 * 
	 * Can be overrided to change the default exec process is onStart -> event loop -> onStop
	 */
	virtual int onExec();
	
	/**
	 * @brief Method called during start process
	 */
	virtual void onStop();
	
private slots:
	/**
	 * @brief Called when the application is quitting
	 */
	void onAboutToQuit();
};

inline Application &Application::getInstance () {
	return *(dynamic_cast<Application*>(instance()));
}
inline void Application::runInstance() {
	getInstance().run();
}
inline void Application::onStart() {

}
inline void Application::onStop() {

}

}

#endif //NODEBUS_APPLICATION_H
