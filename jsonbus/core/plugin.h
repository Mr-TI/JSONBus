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
 * @brief JSONBus : Plugin service management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_SERVICE_H
#define JSONBUS_SERVICE_H

#include <jsonbus/core/exception.h>
#include <jsonbus/core/settings.h>
#include <jsonbus/core/sharedptr.h>

#define jsonbus_declare_plugin(class_name) \
extern "C" {\
	JSONBus::PluginPtr getSingleton () {\
		return new class_name();\
	}\
}

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#include <QString>

namespace JSONBus {

jsonbus_declare_exception(PluginException, Exception);

/**
 * @brief Plugin management.
 */
class JSONBUS_EXPORT Plugin: public QObject, public SharedData {
	Q_OBJECT
private:
	Plugin(const Plugin &other);
	bool m_loaded;
	
public:
	/**
	 * @brief Plugin constructor.
	 */
	Plugin();

	/**
	 * @brief Plugin destructor.
	 */
	virtual ~Plugin() = 0;
	
	/**
	 * @brief Function called on plugin init
	 */
	inline virtual void onInit (Settings &settings) {};
	
	/**
	 * @brief Function called on plugin load
	 */
	inline virtual void onLoad (Settings &settings) {m_loaded = true;};
	
	/**
	 * @brief Function called on plugin unload
	 */
	inline virtual void onUnload () {m_loaded = false;};
	
	/**
	 * @brief Return true if the plugin is loaded
	 */
	inline bool isLoaded() {return m_loaded;};
	
	/**
	 * @brief Function called on plugin request
	 */
	virtual void onRequest(QVariant request) = 0;
	
signals:
	void resultAvailable(QVariant result);
	
};

/// @brief Plugin shared pointer type
typedef SharedPtr<Plugin> PluginPtr;

}

#endif
