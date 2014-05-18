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

/**
 * @brief JSONBus : Bundle activator management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_BUNDLEACTIVATOR_H
#define JSONBUS_BUNDLEACTIVATOR_H

#include <jsonbus/core/exception.h>
#include <jsonbus/core/settings.h>
#include <jsonbus/core/sharedptr.h>
#include <jsonbus/osgi/bundlecontext.h>

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#include <QString>

namespace JSONBus {

jsonbus_declare_exception(BundleActivatorException, Exception);

/**
 * @brief Bundle activator management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class JSONBUS_EXPORT BundleActivator: public SharedData {
	
private:
	BundleActivator(const BundleActivator &other);
	
public:
	
	/**
	 * @brief Bundle activator constructor.
	 */
	BundleActivator();

	/**
	 * @brief Bundle activator destructor.
	 */
	~BundleActivator();
	
	/**
	 * @brief Function called on bundle start
	 */
	virtual void start(BundleContext &context) = 0;
	
	/**
	 * @brief Function called on bundle stop
	 */
	virtual void stop(BundleContext &context) = 0;
	
};

/// @brief Bundle activator shared pointer type
typedef SharedPtr<BundleActivator> BundleActivatorPtr;

inline BundleActivator::BundleActivator() {}
inline BundleActivator::~BundleActivator() {}

}

#endif
