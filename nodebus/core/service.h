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
 * @brief NodeBus : Service management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */

#ifndef NODEBUS_SERVICE_H
#define NODEBUS_SERVICE_H

#include <nodebus/core/exception.h>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#include <QString>

namespace NodeBus {

nodebus_declare_exception(ServiceException, Exception);
nodebus_declare_exception(InvalidPrefixServiceException, ServiceException);
nodebus_declare_exception(InvalidNameServiceException, ServiceException);

/**
 * @brief Service management.
 */
class NODEBUS_EXPORT Service: public QObject {
public:
	/**
	 * @brief Service constructor.
	 * @param name 
	 */
	Service(const QString &prefix, const QString &name);

	/**
	 * @brief Service destructor.
	 */
	~Service();
private:
	/// @brief Service prefix (eg: org.mbedsys.path)
	QString m_prefix;
	/// @brief Service name
	QString m_name;
};

}

#endif
