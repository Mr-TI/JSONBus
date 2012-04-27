/*
    Copyright (c) 2012, Emeric Verschuur <contact@openihs.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the OpenIHS.org nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @brief JSONBus : JSONBus master container management.
 * @file container.h
 * @author Emeric VERSCHUUR <contact@openihs.org>, (C) 2012
 */

#ifndef JSONBUS_CONTAINER_H
#define JSONBUS_CONTAINER_H

#include <QCoreApplication>
#include <QSocketNotifier>
#include <QFile>
#include <fstream>

#include <jsonbus/core/exception.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/sharedlib.h>
#include <jsonbus/core/plugin.h>
#include <jsonbus/core/jsonparserrunnable.h>

using namespace JSONBus;

jsonbus_declare_exception(ContainerException, Exception);

/**
 * @brief JSONBus container management.
 */
class Container : public QCoreApplication {
	Q_OBJECT
public:
	/**
	 * @brief Service constructor.
	 */
	Container(int &argc, char **argv);

	/**
	 * @brief Service destructor.
	 */
	~Container();

	/**
	 * @brief Load the container
	 * @throw Exception on error
	 */
	void launch();

	/**
	 * @brief Get the cli argument object
	 * @return CliArguments reference
	 */
	inline CliArguments &getCliArguments() {
		return m_cliArguments;
	}
	
	/**
	 * @brief Get the container instance
	 * @return Container reference
	 */
	inline static Container &getInstance () {
		return *(static_cast<Container*>(instance()));
	}

	/**
	 * @brief Load the container
	 * @throw Exception on error
	 */
	inline static void launchInstance() {
		getInstance().launch();
	}
	
	bool notify(QObject *rec, QEvent *ev);
	
private slots:
	void onDataAvailable(QVariant data);
	void onResultAvailable(QVariant result);
	
private:
	CliArguments m_cliArguments;
	SharedLib *m_pluginFile;
	Plugin *m_plugin;
	JSONParserRunnable *m_jsonParserTask;
};

#endif //JSONBUS_CONTAINER_H
