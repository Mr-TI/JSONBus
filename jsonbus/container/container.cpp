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
#include <jsonbus/core/settings.h>
#include <jsonbus/core/jsonserializer.h>
#include <jsonbus/core/logger.h>

#include "container.h"

#ifdef WIN32
#	define JSONBUS_SERVICEFILE_PREFIX ""
#	define JSONBUS_SERVICEFILE_SUFFIX ".dll"
#else
#	define JSONBUS_SERVICEFILE_PREFIX "lib"
#	define JSONBUS_SERVICEFILE_SUFFIX ".so"
#endif

jsonbus_declare_application(Container)

Container::Container(int &argc, char **argv)
	: SlaveApplication(argc, argv) {
}

Container::~Container() {}

void Container::onRunLevelSetup() {
	SlaveApplication::onRunLevelSetup();
	
	CliArguments &args = CliArguments::getInstance();
	
	args.define("config",		'c', tr("Set the config path"), "");
	
	args.define("bundle-info",	'i', tr("Get bundle info"));
	args.define("bundle-file",	'f', tr("Bundle file path"), "");
}

void Container::onRunLevelInit() {
	SlaveApplication::onRunLevelInit();
	
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
	
// 	if (args.isEnabled("edit-settings")) {
// 		settings.setup();
// 		throw ExitApplicationException();
// 	}
}

void Container::onRunLevelStart() {
	SlaveApplication::onRunLevelStart();
	m_bundle->start();
}

void Container::onDataAvailable(QVariant data) {
	
}

void Container::onResultAvailable(QVariant result) {
	m_jsonSerialiser.serialize(result);
	std::cout << std::endl;
}
