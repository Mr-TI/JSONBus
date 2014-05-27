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
 * @brief JSONParser : Exception management.
 * @file exception.h
 * @author Emeric VERSCHUUR <emericv@openihs.org>, (C) 2012
 */
#ifndef JSONPARSER_EXCEPTION_H
#define JSONPARSER_EXCEPTION_H

#include <QString>
#include <QObject>
#include <QtCore>

#define jsonparser_declare_exception(ename, eparent) \
class JSONBUS_EXPORT ename:public eparent {\
public:\
    inline ename(const QString &msg = ""):eparent(msg) {}\
	\
    inline virtual ~ename() throw() {}\
	\
	inline virtual void raise() const {\
		throw *this;\
	}\
	\
	inline virtual ename *clone() const {\
		return new ename(*this);\
	}\
};

namespace jsonparser {

/**
 * This class can manage exceptions.
 * @brief JSONBus : Exceptions.
 */
class Exception : public QtConcurrent::Exception {
public:
	/**
	 * @brief Exception constructor.
	 * @param message exeption message.
	 */
	inline Exception(const QString &message = ""): m_message(message) {}
	/**
	 * @brief Exception constructor.
	 * @param message exeption message.
	 */
	inline Exception(const Exception &exception): m_message(exception.m_message) {}

	/**
	 * @brief Exception destructor.
	 */
	inline virtual ~Exception() throw() { }

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
	inline const char *what() const throw() {
		return m_message.toAscii().data();
	}  
	
	inline void raise() const {
		throw *this;
	}
	
	inline Exception *clone() const {
		return new Exception(*this);
	}
private:
	QString m_message;
};

jsonparser_declare_exception(ErrorException, Exception);
jsonparser_declare_exception(EOFException, Exception);

}

#endif //JSONPARSER_EXCEPTION_H
