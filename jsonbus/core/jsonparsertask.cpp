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


#include "jsonparsertask.h"
#include <typeinfo>
#include "common.h"
#include "logger.h"

namespace JSONBus {

JSONParserTask::JSONParserTask(int fd, QObject* parent)
: JSONParser(fd, parent), m_stop(false) {

}

JSONParserTask::JSONParserTask(std::istream& stream, QObject* parent)
: JSONParser(stream, parent), m_stop(false) {

}

JSONParserTask::JSONParserTask(QTextStream& stream, QObject* parent)
: JSONParser(stream, parent), m_stop(false) {

}

JSONParserTask::JSONParserTask(QIODevice& input, QObject* parent)
: JSONParser(input, parent), m_stop(false) {

}

void JSONParserTask::run() {
	try {
		while (1) {
			emit dataAvailable(parse());
		}
	} catch (JSONParserException &e) {
		if (m_stop) {
			logFine() << demangle(typeid(*this).name()) << " leaving main loop normally";
		} else {
			logCrit() << demangle(typeid(*this).name()) << " leaving main loop after throwing an instance of '" << demangle(typeid(e).name()) << "'";
			if (!e.message().isEmpty())
				logCrit() << "  what(): " << e.message();
		}
	}
	emit terminated();
}

void JSONParserTask::terminate() {
	m_stop = true;
	disable();
}


}

