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
 * @brief NodeBus proxy : master service management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_MASTER_H
#define JSONBUS_MASTER_H

#include <nodebus/core/application.h>
#include <nodebus/core/peeradmin.h>

using namespace NodeBus;

nodebus_declare_exception(MasterException, Exception);

class Intf;
typedef SharedPtr<Intf> IntfPtr;

class Intf: public SharedData {
public:
	Intf(const QVariantMap &node);
	bool mark;
	bool completed;
	QString name;
	QVariantMap members;
	QStringList parents;
	QMap<QString, IntfPtr > ancestors;
};

/**
 * @brief Dynamic library management.
 */
class IDLc: public Application {
private:
	SettingsPtr m_settings;
	QMap<QString, IntfPtr > m_symTbl;
	QStringList m_errors;
	void processIntf(IntfPtr &intf);
	void link(const QVariantList &elements);
public:
	/**
	 * @brief Service constructor.
	 */
	IDLc(int &argc, char **argv);

	/**
	 * @brief Service destructor.
	 */
	~IDLc();
	
	virtual void onInit();
	virtual int onExec();
};

#endif
