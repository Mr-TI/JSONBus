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
 * @brief JSONBus : JSON task parser management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */


#ifndef JSONBUS_JSONPARSERTASK_H
#define JSONBUS_JSONPARSERTASK_H

#include <QRunnable>
#include <jsonbus/core/jsonparser.h>

namespace JSONBus {

class JSONParserTask : public JSONBus::JSONParser, public QRunnable {
	Q_OBJECT
public:
	
	/**
	 * @brief JSONParserTask constructor.
	 * @param fd File descriptor
	 * @param parent Parent object
	 */
	JSONParserTask(const StreamChannelPtr &channel, QObject* parent = 0);
	
	/**
	 * @brief Task main
	 */
	virtual void run();
signals:
	void dataAvailable(QVariant data);
	void terminated();
public slots:
	void cancel();
private:
	bool m_stop;
};

}

#endif // JSONBUS_JSONPARSERTASK_H
