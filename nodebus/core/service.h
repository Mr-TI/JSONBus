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
 * @brief NodeBus : Service management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
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
