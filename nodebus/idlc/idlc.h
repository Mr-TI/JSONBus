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
