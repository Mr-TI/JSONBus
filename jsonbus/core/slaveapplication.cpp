/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@openihs.org>
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

#include <unistd.h>
#include <typeinfo>
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/jsonparsertask.h>
#include "slaveapplication.h"

namespace JSONBus {

SlaveApplication::SlaveApplication(int &argc, char **argv)
	: Application(argc, argv) {
}

SlaveApplication::~SlaveApplication() {
}

void SlaveApplication::onStart() {
	JSONParserTask *jsonParserTask = new JSONParserTask(STDIN_FILENO);
	
	connect(jsonParserTask, SIGNAL(terminated()), this, SLOT(quit()));
	connect(jsonParserTask, SIGNAL(dataAvailable(QVariant)), this, SLOT(onDataAvailable(QVariant)));
	connect(this, SIGNAL(aboutToQuit()), jsonParserTask, SLOT(terminate()));
	
	QThreadPool::globalInstance()->start(jsonParserTask);
}

}
