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

#include <QStringList>
#include <nodebus/core/common.h>
#include <nodebus/core/cliarguments.h>
#include <nodebus/core/settings.h>
#include <nodebus/core/logger.h>
#include <nodebus/core/serializer.h>
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
: mark(false), completed(false), name(node[NODE_KEY_NAME].toString()) {
	for (auto &member: node[NODE_KEY_MEMBERS].toList()) {
		members[member.toMap()[NODE_KEY_NAME].toString()] = member;
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
		QString memberName = member.toMap()[NODE_KEY_NAME].toString();
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
			QString memberName = member.toMap()[NODE_KEY_NAME].toString();
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
			for (auto elt: Parser::fromFile(file, formatFromName(QFileInfo(file).suffix())).toMap()[NODE_KEY_MEMBERS].toList()) {
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
		resProps[NODE_KEY_NODE_TYPE] = TYPE_NODE_FRAGMENT;
	} else {
		resProps[NODE_KEY_NODE_TYPE] = TYPE_NODE_BASE;
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
		Serializer::toFile(outFile, resProps, format);
	return 0;
}
