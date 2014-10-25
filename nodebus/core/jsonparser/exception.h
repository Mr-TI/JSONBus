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
 * @brief JSONParser : Exception management.
 * @file exception.h
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */
#ifndef JSONPARSER_EXCEPTION_H
#define JSONPARSER_EXCEPTION_H

#include <QString>
#include <QObject>
#include <QtCore>

#define jsonparser_declare_exception(ename, eparent) \
class NODEBUS_EXPORT ename:public eparent {\
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
 * @brief NodeBus : Exceptions.
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
