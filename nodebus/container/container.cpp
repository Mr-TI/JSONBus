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
#include <nodebus/core/settings.h>
#include <nodebus/core/logger.h>
#include <nodebus/core/iochannel.h>

#include "container.h"
#include <unistd.h>

#ifdef WIN32
#	define NODEBUS_SERVICEFILE_PREFIX ""
#	define NODEBUS_SERVICEFILE_SUFFIX ".dll"
#else
#	define NODEBUS_SERVICEFILE_PREFIX "lib"
#	define NODEBUS_SERVICEFILE_SUFFIX ".so"
#endif

nodebus_declare_application(Container)

Container::Container(int &argc, char **argv)
	: SlaveApplication(argc, argv), m_jsonSerialiser(new IOChannel(STDOUT_FILENO)) {
}

Container::~Container() {}

void Container::onInit() {
	CliArguments &args = CliArguments::getInstance();
	
// 	args.define("config",		'c', tr("Set the config path"), "");
	
	args.define("bundle-info",	'i', tr("Get bundle info"));
	args.define("bundle-file",	'f', tr("Bundle file path"), "");
	args.define("bundle-start",	's', tr("Start the bundle"));
}

void Container::onStart() {
	CliArguments &args = CliArguments::getInstance();
	
	QString bundlePath = args.getValue("bundle-file").toString();
	
	// Checking arguments
	if (bundlePath.isEmpty()) {
		throw ContainerException("Undefinied bundle path");
	}
	
	m_bundle = new Bundle(bundlePath);
	
	if (args.isEnabled("bundle-info")) {
		logInfo() << "Bundle information:" << m_bundle->manifest();
		throw ExitApplicationException();
	}
	
	if (args.isEnabled("bundle-start")) {
		m_bundle->start();
	}
	
	SlaveApplication::onStart();
}

void Container::onDataAvailable(QVariant data) {
	
}

void Container::onResultAvailable(QVariant result) {
	m_jsonSerialiser.serialize(result);
	std::cout << std::endl;
}
