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

#include <QStringList>
#include <nodebus/core/common.h>
#include <nodebus/core/cliarguments.h>
#include <nodebus/core/settings.h>
#include <nodebus/core/logger.h>
#include <nodebus/core/sslserversocketchannel.h>
#include <nodebus/core/ssliochannel.h>
#include <nodebus/core/sslcontext.h>
#include <nodebus/core/peeradmin.h>
#include <nodebus/core/serializer.h>
#include <nodebus/core/filechannel.h>
#include "idlc.h"

nodebus_declare_master_service(IDLc)

IDLc::IDLc(int &argc, char **argv)
	: Application(argc, argv) {
}

IDLc::~IDLc() {
}

void IDLc::onInit() {
	CliArguments &args = CliArguments::getInstance();
	args.define("output-file", 'o', "Output file path (default: out.<ext>)");
// 	args.define("input-format", 'i', "Output format (IDL, JSON, BSON or BCON, default: automatically detected from the extention)");
	args.define("output-format", 'f', "Output format (JSON, BSON or BCON, default: BCON)");
	args.define("compile", 'c', "Compile but not link");
	args.setExtraArgsLegend("<input file1>[ <input file1>[ <input file3>[...]]]");
}

void IDLc::onExec() {
	CliArguments &args = CliArguments::getInstance();
	const QStringList &files = args.extraArgs();
	if (files.isEmpty())
		throw ApplicationException("No input file");
	QString formatStr = args.getValue("output-format").toString();
	FileFormat format;
	QString outName("out.");
	if (formatStr == "BCON" || formatStr == "") {
		format = BCON;
		outName.append("bcon");
	} else if (formatStr == "BSON") {
		format = BSON;
		outName.append("bson");
	} else if (formatStr == "JSON") {
		format = JSON;
		outName.append("json");
	}
	QVariantList resList;
	for (auto file: files) {
		
	}
	Serializer(new FileChannel(outName, O_CREAT | O_TRUNC | O_WRONLY), format).serialize(resList);
}
