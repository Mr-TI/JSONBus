/*
    Copyright (c) 2012, Emeric Verschuur <contact@openihs.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the OpenIHS.org nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @brief JSONBus : JSON task parser management.
 * @file jsonparsertask.h
 * @author Emeric VERSCHUUR <contact@openihs.org>, (C) 2012
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
	JSONParserTask(int fd, QObject* parent = 0);
	
	/**
	 * @brief JSONParser constructor.
	 * @param stream STD input stream to get data from
	 * @param parent Parent object
	 */
	JSONParserTask(std::istream& stream, QObject* parent = 0);
	
	/**
	 * @brief JSONParser constructor.
	 * @param stream Text stream to get data from
	 * @param parent Parent object
	 */
	JSONParserTask(QTextStream& stream, QObject* parent = 0);
	
	/**
	 * @brief JSONParser constructor.
	 * @param input Device to get data from
	 * @param parent Parent object
	 */
	JSONParserTask(QIODevice& input, QObject* parent = 0);
	
	/**
	 * @brief Task main
	 */
	virtual void run();
signals:
	void dataAvailable(QVariant data);
	void terminated();
public slots:
	void terminate();
private:
	bool m_stop;
};

}

#endif // JSONBUS_JSONPARSERTASK_H
