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
 * @brief JSONBus : Bundle management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_BUNDLE_H
#define JSONBUS_BUNDLE_H

#include <jsonbus/core/exception.h>
#include <jsonbus/core/settings.h>
#include <jsonbus/core/sharedptr.h>
#include <jsonbus/core/bundlecontext.h>

#define MANIFEST_BUNDLE_NAME(name) \
extern "C" QString __manifest_BundleName = name

#define MANIFEST_BUNDLE_SYMBOLIC_NAME(name) \
extern "C" QString __manifest_BundleSymbolicName = name

#define MANIFEST_BUNDLE_VERSION(version) \
extern "C" QString __manifest_BundleVersion = version

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#include <QString>

namespace JSONBus {

jsonbus_declare_exception(BundleException, Exception);

/**
 * @brief Bundle management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */
class JSONBUS_EXPORT Bundle: public SharedData {
public:
	
	/// @brief Bundle state
	enum State {
		/// @brief Uninstalled
		UNINSTALLED,
		/// @brief Installed
		INSTALLED,
		/// @brief Starting
		STARTING,
		/// @brief Active (started)
		ACTIVE,
		/// @brief Stopping
		STOPPING
	};
	
private:
	Bundle(const Bundle &other);
	
	/// @brief Bundle state
	State m_state;
	
	/// @brief Bundle context
	BundleContext m_context;
	
public:
	
	/**
	 * @brief Bundle constructor.
	 */
	Bundle();

	/**
	 * @brief Bundle destructor.
	 */
	~Bundle();
	
	/**
	 * @brief Function called on plugin load
	 */
	virtual void start() throw(BundleException) = 0;
	
	/**
	 * @brief Function called on plugin unload
	 */
	virtual void stop() throw(BundleException) = 0;
	
	/**
	 * @brief Return the bundle state
	 */
	State state();
	
};

/// @brief Bundle shared pointer type
typedef SharedPtr<Bundle> BundlePtr;

inline Bundle::Bundle(): m_state(UNINSTALLED), m_context(*this) {}
inline Bundle::State Bundle::state() {
	return m_state;
}
inline Bundle::~Bundle() {}

}

#endif
