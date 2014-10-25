/*
 * Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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
#include <nodebus/bundle/bundle.h>
#include <nodebus/bundle/bundleactivator.h>

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
