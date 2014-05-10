/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@openihs.org>
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
 * @brief JSONBus : Service management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_SERVICE_H
#define JSONBUS_SERVICE_H

#include <jsonbus/core/exception.h>

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#include <QString>

namespace JSONBus {

jsonbus_declare_exception(ServiceException, Exception);
jsonbus_declare_exception(InvalidPrefixServiceException, ServiceException);
jsonbus_declare_exception(InvalidNameServiceException, ServiceException);

/**
 * @brief Service management.
 */
class JSONBUS_EXPORT Service: public QObject {
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
	/// @brief Service prefix (eg: org.openihs.path)
	QString m_prefix;
	/// @brief Service name
	QString m_name;
};

}

#endif
