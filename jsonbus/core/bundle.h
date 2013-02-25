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

#define jsonbus_declare_plugin(class_name) \
extern "C" {\
	JSONBus::BundlePtr getSingleton () {\
		return new class_name();\
	}\
}

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#include <QString>

namespace JSONBus {

jsonbus_declare_exception(BundleException, Exception);

/**
 * @brief Bundle management.
 */
class JSONBUS_EXPORT Bundle: public SharedData {
public:
	enum State {
		UNINSTALLED,
		INSTALLED,
		STARTING,
		ACTIVE,
		STOPPING
	};
	
private:
	Bundle(const Bundle &other);
	State m_state;
	
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
	 * @brief Function called on plugin init
	 */
	virtual void install(Settings &settings) {};
	
	/**
	 * @brief Function called on plugin load
	 */
	virtual void start(Settings &settings) = 0;
	
	/**
	 * @brief Function called on plugin unload
	 */
	virtual void stop() = 0;
	
	/**
	 * @brief Return true if the plugin is loaded
	 */
	State state();
	
};

/// @brief Bundle shared pointer type
typedef SharedPtr<Bundle> BundlePtr;

inline Bundle::Bundle() {}
inline Bundle::State Bundle::state() {
	return m_state;
}
inline Bundle::~Bundle() {}

}

#endif
