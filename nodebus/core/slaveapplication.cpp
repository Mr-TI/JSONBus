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

#include <unistd.h>
#include <typeinfo>
#include <QStringList>
#include <nodebus/core/common.h>
#include "slaveapplication.h"

namespace NodeBus {

SlaveApplication::SlaveApplication(int &argc, char **argv)
	: Application(argc, argv) {
}

SlaveApplication::~SlaveApplication() {
}

void SlaveApplication::onStart() {
// 	JSONParserTask *jsonParserTask = new JSONParserTask(new IOChannel(STDIN_FILENO));
// 	
// 	connect(jsonParserTask, SIGNAL(terminated()), this, SLOT(quit()));
// 	connect(jsonParserTask, SIGNAL(dataAvailable(QVariant)), this, SLOT(onDataAvailable(QVariant)));
// 	connect(this, SIGNAL(aboutToQuit()), jsonParserTask, SLOT(cancel()));
// 	
// 	QThreadPool::globalInstance()->start(jsonParserTask);
}

}
