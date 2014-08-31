/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
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
 * @brief NodeBus : NodeBus master service management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_MASTER_H
#define NODEBUS_MASTER_H

#include <nodebus/core/application.h>
#include <nodebus/bundle/bundle.h>

using namespace NodeBus;

nodebus_declare_exception(MasterException, Exception);

/**
 * @brief Dynamic library management.
 */
class Master : public Application {
private:
	SettingsPtr m_settings;
	QMap<QString, BundlePtr> m_bundles;
public:
	/**
	 * @brief Service constructor.
	 */
	Master(int &argc, char **argv);

	/**
	 * @brief Service destructor.
	 */
	~Master();
	
protected:
	virtual void onInit();
	virtual void onStart();
};

#endif
