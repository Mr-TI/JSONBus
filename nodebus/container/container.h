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
 * @brief NodeBus : NodeBus container management.
 * @file container.h
 * @author Emeric VERSCHUUR <emericv@mbedsys.org>, (C) 2012
 */

#ifndef NODEBUS_CONTAINER_H
#define NODEBUS_CONTAINER_H

#include <nodebus/core/sharedlib.h>
#include <nodebus/core/serializer.h>
#include <nodebus/core/slaveapplication.h>
#include <nodebus/osgi/bundle.h>
#include <nodebus/osgi/bundleactivator.h>

using namespace NodeBus;

nodebus_declare_exception(ContainerException, ApplicationException);

/**
 * @brief NodeBus container management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
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
	virtual void onInit();
	virtual void onStart();
	
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
	Serializer m_jsonSerialiser;
};

#endif //NODEBUS_CONTAINER_H
