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
 * @brief NodeBus proxy : master service management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */

#ifndef JSONBUS_MASTER_H
#define JSONBUS_MASTER_H

#include <nodebus/core/application.h>
#include <nodebus/nio/peeradmin.h>

using namespace NodeBus;

nodebus_declare_exception(MasterException, Exception);

/**
 * @brief Dynamic library management.
 */
class Proxy : public Application {
private:
	SettingsPtr m_settings;
public:
	/**
	 * @brief Service constructor.
	 */
	Proxy(int &argc, char **argv);

	/**
	 * @brief Service destructor.
	 */
	~Proxy();
	
	PeerAdmin &getPeerAdmin();
	
	/**
	 * @brief Get the Proxy instance
	 * @return Application reference
	 */
	static Proxy &getInstance ();
	
protected:
	virtual void onInit();
	virtual void onStart();
	
private:
	PeerAdmin m_socketAdmin;
};

inline Proxy& Proxy::getInstance() {
	return (Proxy&)Application::getInstance();
}

inline PeerAdmin& Proxy::getPeerAdmin() {
	return m_socketAdmin;
}

#endif
