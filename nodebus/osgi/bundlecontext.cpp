/*
 *   Copyright 2014 Emeric Verschuur <emericv@mbedsys.org>
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include "bundlecontext.h"
#include <nodebus/core/logger.h>
#include <qmetaobject.h>

using namespace NodeBus;

void BundleContext::registerService(QObject& service) {
	const QMetaObject *metaobject = service.metaObject();
	QString name = QString(metaobject->className()).replace("::",".");
	logFiner() << "Register Service:\n\tName\t: " << name;
	for (int i = metaobject->methodOffset(); i < metaobject->methodCount(); i++) {
		logFiner() << "\tMethod\t: " << metaobject->method(i).signature();
	}
}
