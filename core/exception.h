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
 * @brief JSONBus : Exception management.
 * @file exception.h
 * @author Emeric VERSCHUUR <contact@openihs.org>, (C) 2012
 */
#ifndef UNICOMCTRL_EXCEPTION_H
#define UNICOMCTRL_EXCEPTION_H

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#include <QString>
#include <QObject>

#define jsonbus_declare_exception(name, parent)\
class JSONBUS_EXPORT name:public parent {\
public:\
    inline name(const QString &msg):parent(msg) {}\
};

namespace JSONBus {

/**
 * This class can manage exceptions.
 * @brief JSONBus : Exceptions.
 */
class JSONBUS_EXPORT Exception : public QObject {
public:
	/**
	 * @brief Exception constructor.
	 * @param message exeption message.
	 */
	inline Exception(const QString &message): m_message(message) {}
	/**
	 * @brief Exception constructor.
	 * @param message exeption message.
	 */
	inline Exception(const Exception &exception): m_message(exception.m_message) {}

	/**
	 * @brief Exception destructor.
	 */
	inline ~Exception() {}

	/**
	 * @brief Get the exeption message.
	 * @return QString message.
	 */
	inline const QString message() const {
		return m_message;
	}

	/**
	 * @brief Get the exeption message.
	 * @return QString message.
	 */
	inline const QString getMessage() const {
		return m_message;
	}
private:
	QString m_message;
};

}

#endif
