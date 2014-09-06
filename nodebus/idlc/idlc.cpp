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
#include <nodebus/core/idlparser/constants.h>
#include <nodebus/core/parser.h>
#include "idlc.h"

nodebus_declare_master_service(IDLc)

using namespace idlparser;

IDLc::IDLc(int &argc, char **argv)
	: Application(argc, argv) {
}

IDLc::~IDLc() {
}

void IDLc::onInit() {
	CliArguments &args = CliArguments::getInstance();
	args.define("display", 'd', "Display result in JSON format");
	args.define("dry-run", 'n', "Just try to compile/link and do not generate ouput file");
	args.define("output-file", 'o', "Output file path (default: metadb.<ext>)", "");
// 	args.define("input-format", 'i', "Output format (IDL, JSON, BSON or BCON, default: automatically detected from the extention)");
	args.define("output-format", 'f', "Output format (JSON, BSON or BCON)", "AUTO");
	args.define("compile", 'c', "Compile but not link");
	args.setHelpHeader(QString("\n  IDL compiler tool (Built on ") + __DATE__ + " " + __TIME__ + ")\n  Author: Emeric Verschuur <emericv@mbedsys.org>, Copyright 2014 MBEDSYS SAS");
	args.setExtraArgsLegend("<input file1>[ <input file1>[ <input file3>[...]]]");
}

inline FileFormat formatFromName(QString fmtStr) {
	fmtStr = fmtStr.toUpper();
	if (fmtStr == "IDL") {
		return IDL;
	} else if (fmtStr == "BCON") {
		return BCON;
	} else if (fmtStr == "BSON") {
		return BSON;
	} else if (fmtStr == "JSON") {
		return JSON;
	} else {
		throw ApplicationException("Invalid format " + fmtStr);
	}
}

inline Intf::Intf(const QVariantMap& node)
: mark(false), completed(false), name(node[NODE_KEY_SNAME].toString()) {
	for (auto &member: node[NODE_KEY_MEMBERS].toList()) {
		members[member.toMap()[NODE_KEY_SNAME].toString()] = member;
	}
	for (auto &parent: node[NODE_KEY_PARENTS].toList()) {
		parents.append(parent.toString());
	}
}

void IDLc::processIntf(IntfPtr &intf) {
	if (intf->mark) {
		m_errors.append("Cyclic inheritance detected for the interface " + intf->name);
		return;
	}
	if (intf->completed) return;
	intf->mark = true;
	for (auto parent: intf->parents) {
		if (!m_symTbl.contains(parent)) {
			m_errors.append("Undefined symbol " + parent);
			continue;
		}
		IntfPtr intfP = m_symTbl[parent];
		processIntf(intfP);
		for (auto ancestor : intfP->ancestors) {
			intf->ancestors[ancestor->name] = ancestor;
		}
		intf->ancestors[intfP->name] = intfP;
	}
	intf->mark = false;
	QMap<QString, QString> symMap;
	for (auto member : intf->members) {
		QString memberName = member.toMap()[NODE_KEY_SNAME].toString();
		if (symMap.contains(memberName)) {
			m_errors.append("Undefined symbol " + intf->name + "::" + memberName);
			continue;
		}
		symMap[memberName] = intf->name;
	}
	for (auto it = intf->ancestors.begin(); it != intf->ancestors.end(); it++) {
		QString nameP = it.key();
		auto membersP = it.value()->members;
		for (auto member : membersP) {
			QString memberName = member.toMap()[NODE_KEY_SNAME].toString();
			if (symMap.contains(memberName)) {
				m_errors.append("Interface " + intf->name + ": conflict between the two symbols " +
				symMap[memberName] + NAMESPACE_SEP + memberName + " and " + nameP + NAMESPACE_SEP + memberName);
				continue;
			}
			symMap[memberName] = nameP;
		}
	}
	intf->completed = true;
}

inline void IDLc::link(const QVariantList &elements) {
	for (auto &elt: elements) {
		IntfPtr intf = new Intf(elt.toMap());
		if (m_symTbl.contains(intf->name)) {
			m_errors.append("Duplicated symbol " + intf->name);
			continue;
		}
		m_symTbl[intf->name] = intf;
	}
	for (auto intf : m_symTbl) {
		processIntf(intf);
	}
	if (!m_errors.isEmpty()) {
		throw ErrorParserException(m_errors.join("\n"));
	}
}

int IDLc::onExec() {
	CliArguments &args = CliArguments::getInstance();
	const QStringList &files = args.extraArgs();
	if (files.isEmpty()) {
		logCrit() << "No input file";
		return 1;
	}
	QString formatStr = args.getValue("output-format").toString();
	FileFormat format;
	QVariantMap resProps;
	resProps[NODE_KEY_VERSION] = 1;
	QVariantList resList;
	QString outFile(args.getValue("output-file").toString());
	try {
		for (auto file: files) {
			for (auto elt: Parser::parseFile(file, formatFromName(QFileInfo(file).suffix())).toMap()[NODE_KEY_MEMBERS].toList()) {
				resList.append(elt);
			}
		}
		if (formatStr == "AUTO") {
			if (outFile.isEmpty()) {
				outFile = "metadb.bcon";
				format = BCON;
			} else {
				format = formatFromName(QFileInfo(outFile).suffix());
			}
		} else {
			format = formatFromName(formatStr);
			if (outFile.isEmpty()) {
				outFile = "metadb." + formatStr.toLower();
			}
		}
	} catch (Exception &e) {
		logCrit() << "Compilation failed:\n" << e.message();
		return 1;
	}
	if (args.isEnabled("compile")) {
		resProps[NODE_KEY_TYPE] = TYPE_NODE_FRAGMENT;
	} else {
		resProps[NODE_KEY_TYPE] = TYPE_NODE_BASE;
		try {
			link(resList);
		} catch (Exception &e) {
			logCrit() << "Link failed:\n" << e.message();
			return 1;
		}
	}
	resProps[NODE_KEY_MEMBERS] = resList;
	if (args.isEnabled("display"))
		logInfo() << Serializer::toJSONString(resProps, Serializer::INDENT(2));
	if (!args.isEnabled("dry-run"))
		Serializer(new FileChannel(outFile, O_CREAT | O_TRUNC | O_WRONLY), format).serialize(resProps);
	return 0;
}
