/*
 * Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <typeinfo>
#include <QStringList>

#include <nodebus/core/common.h>
#include <nodebus/core/settings.h>
#include <nodebus/core/logger.h>
#include <nodebus/nio/iochannel.h>

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
