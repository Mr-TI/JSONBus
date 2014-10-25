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
 * @brief NodeBus : NodeBus master service management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
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
