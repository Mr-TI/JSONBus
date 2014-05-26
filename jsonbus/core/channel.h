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

#ifndef JSONPARSER_SELECTABLECHANNEL_H
#define JSONPARSER_SELECTABLECHANNEL_H

#include <jsonbus/core/shareddata.h>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/sharedptr.h>
#include <jsonbus/core/selector.h>

/**
 * @namespace
 */
namespace JSONBus {

class SelectionKey;
class Selector;

/**
 * @brief Abstract channel
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class Channel: public QObject, public SharedData {
	Q_OBJECT
	friend class Selector;
	friend class SelectionKey;
public:
	/**
	 * @brief Channel constructor
	 */
	Channel();
	
	/**
	 * @brief Channel destructor
	 */
	virtual ~Channel() = 0;
	
	/**
	 * @brief Return if the channel is open
	 * @return true if the channel is open, otherwise false
	 */
	virtual bool isOpen();
	
	SharedPtr<SelectionKey> registerTo(JSONBus::Selector& selector, int options, GenericPtr attachement=NULL);
	
	SharedPtr<SelectionKey> keyFor(Selector &selector);

public slots:
	/**
	 * @brief Close the channel
	 */
	virtual void close();

signals:
	void closed();
	
protected:
	virtual int &fd() = 0;
	virtual void updateStatus(int events) = 0;
	bool m_open;
	
private:
	QMap<Selector*, SharedPtr<SelectionKey> > m_keys;
};

inline bool Channel::isOpen() {
	return m_open;
}

typedef SharedPtr<Channel> ChannelPtr;

}

#endif // JSONPARSER_SELECTABLECHANNEL_H
