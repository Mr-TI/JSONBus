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
 * @brief JSONBus : JSONBus container management.
 * @file container.h
 * @author Emeric VERSCHUUR <emericv@openihs.org>, (C) 2012
 */

#ifndef JSONBUS_CONTAINER_H
#define JSONBUS_CONTAINER_H

#include <jsonbus/core/sharedlib.h>
#include <jsonbus/core/plugin.h>
#include <jsonbus/core/jsonserializer.h>
#include <jsonbus/core/slaveapplication.h>
#include <jsonbus/core/bundle.h>
#include <jsonbus/core/bundleactivator.h>

using namespace JSONBus;

jsonbus_declare_exception(ContainerException, ApplicationException);

/**
 * @brief JSONBus container management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */
class Container : public SlaveApplication {
	Q_OBJECT
public:
	/**
	 * @brief Service constructor.
	 */
	Container(int &argc, char **argv);

	/**
	 * @brief Service destructor.
	 */
	~Container();
	
protected:
	virtual void onRunLevelSetup();
	virtual void onRunLevelInit();
	virtual void onRunLevelStart();
	
protected slots:
	/**
	 * @brief Inpout data treatment
	 * @param data
	 */
	virtual void onDataAvailable(QVariant data);
	
	/**
	 * @brief Result data treatment
	 * @param data
	 */
	void onResultAvailable(QVariant result);
	
private:
	SharedLibPtr m_libFile;
	BundlePtr m_bundle;
	JSONSerializer m_jsonSerialiser;
};

#endif //JSONBUS_CONTAINER_H
