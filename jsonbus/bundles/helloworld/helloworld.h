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

/**
 * @brief Sample JSONBus HelloWorld bundle.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_BUNDLES_HELLOWORLD_H
#define JSONBUS_BUNDLES_HELLOWORLD_H

#include <jsonbus/core/exception.h>
#include <jsonbus/core/bundleactivator.h>

using namespace JSONBus;

class HelloWorld: public BundleActivator {
	virtual void start(BundleContext& context) throw(BundleActivatorException);
	virtual void stop(BundleContext& context) throw(BundleActivatorException);
};

#endif // JSONBUS_BUNDLES_HELLOWORLD_H
