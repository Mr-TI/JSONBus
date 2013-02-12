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
 * @brief Sample JSONBus echo plugin.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_SERVICES_ECHO_H
#define JSONBUS_SERVICES_ECHO_H

#include <jsonbus/core/exception.h>
#include <jsonbus/core/plugin.h>

using namespace JSONBus;

/**
 * @brief Sample JSONBus echo plugin.
 */
class Echo: public Plugin {
public:
	/**
	 * @brief Echo service constructor.
	 */
	Echo();

	/**
	 * @brief Echo service destructor.
	 */
	~Echo();
	
	/**
	 * @brief Function called on plugin init
	 */
	virtual void onInit (Settings &settings);
	
	/**
	 * @brief Function called on plugin load
	 */
	virtual void onLoad (Settings &settings);
	
	/**
	 * @brief Function called on plugin unload
	 */
	virtual void onUnload ();
	
	/**
	 * @brief Function called on plugin request
	 */
	virtual void onRequest(QVariant request);
private:
	QString m_fieldName;
	QString m_helloMsg;
};

#endif // JSONBUS_PLUGIN_ECHO_H
