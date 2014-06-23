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

#include <nodebus/core/common.h>
#include <nodebus/core/logger.h>

#include "helloworld.h"
#include "helloimpl.h"

void HelloWorld::start(BundleContext& context) {
	logInfo() << "HelloWorld bundle starting...";
	HelloImpl helloSrv;
	context.registerService(helloSrv);
}

void HelloWorld::stop(BundleContext& context) {
	logInfo() << "HelloWorld bundle stopping...";
}
