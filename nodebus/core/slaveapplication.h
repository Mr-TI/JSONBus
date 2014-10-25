/*
 * Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * @brief NodeBus : NodeBus slave application management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_SLAVEAPPLICATION_H
#define NODEBUS_SLAVEAPPLICATION_H

#include <QCoreApplication>
#include <nodebus/core/application.h>
#include <nodebus/core/settings.h>

namespace NodeBus {

/**
 * @brief NodeBus container management.
 */
class SlaveApplication : public Application {
	Q_OBJECT
public:
	/**
	 * @brief Service constructor.
	 */
	SlaveApplication(int &argc, char **argv);

	/**
	 * @brief Service destructor.
	 */
	~SlaveApplication();
	
protected:
	/**
	 * @brief Method called during start process
	 */
	virtual void onStart();
	
protected slots:
	/**
	 * @brief Inpout data treatment
	 * @param data
	 */
	virtual void onDataAvailable(QVariant data) = 0;
};

}

#endif //NODEBUS_SLAVEAPPLICATION_H
