/*
 *   Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
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

#ifndef JSONBUS_LOGGER_H
#define JSONBUS_LOGGER_H

#include <QMap>
#include <QString>
#include <QTextStream>
#include <qglobal.h>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/common.h>

namespace JSONBus {

/**
 * @brief JSONBus : Logger
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */
class Logger {
public:
	/// @brief Log level
	typedef enum {
		/// @brief OFF is a special level that can be used to turn off logging.
		OFF,

		/// @brief CRITICAL is a message level indicating a serious failure.
		CRITICAL,

		/// @brief WARNING is a message level indicating a potential problem.
		WARNING,

		/// @brief INFO is a message level for informational messages.
		INFO,

		/// @brief CONFIG is a message level for static configuration messages.
		CONFIG,

		/// @brief FINE is a message level providing tracing information.
		FINE,

		/// @brief FINER indicates a fairly detailed tracing message.
		FINER,

		/// @brief FINEST indicates a highly detailed tracing message.
		FINEST,

		/// @brief ALL indicates that all messages should be logged.
		ALL
	} Level;

	/**
	 * @brief Loggable object
	 */
	class Loggable {
		friend class Logger;
	protected:
		/**
		 * @brief Write the human readable representation of the frame field on the logger
		 * 
		 * @param logger Logger object used for display
		 */
		virtual void writeToLogger(Logger &logger) const = 0;
	};

private:
	static const char *LEVEL_HDRS[][8];
	static const char *LEVEL_FOOT[];
	static const char **levelHdrs;
	static const char *levelFoot;
	static Level globalLevel;

	QString m_buffer;
	QTextStream m_stream;
	Level m_level;

	void append(QString str);
	void appendln();

public:
	/**
	 * @brief Log entry contructor
	 * 
	 * @param level Level
	 */
	Logger(Level level);
	
	/**
	 * @brief Log entry contructor by copy
	 */
	Logger(const Logger &other);
	
	/**
	 * @brief Log entry destructor
	 */
	~Logger();
	
	/**
	 * @brief Write a QChar value to the logger
	 * 
	 * @param t QChar value
	 */
	Logger &operator<<(QChar t);
	
	/**
	 * @brief Write a QBool value to the logger
	 * 
	 * @param t QBool value
	 */
	Logger &operator<<(QBool t);
	
	/**
	 * @brief Write a boolean value to the logger
	 * 
	 * @param t boolean value
	 */
	Logger &operator<<(bool t);
	
	/**
	 * @brief Write a char value to the logger
	 * 
	 * @param t char value
	 */
	Logger &operator<<(char t);
	
	/**
	 * @brief Write a signed short integer value to the logger
	 * 
	 * @param t signed short integer value
	 */
	Logger &operator<<(signed short t);
	
	/**
	 * @brief Write a unsigned short integer value to the logger
	 * 
	 * @param t unsigned short integer value
	 */
	Logger &operator<<(unsigned short t);
	
	/**
	 * @brief Write a signed integer value to the logger
	 * 
	 * @param t signed integer value
	 */
	Logger &operator<<(signed int t);
	
	/**
	 * @brief Write a unsigned integer value to the logger
	 * 
	 * @param t unsigned integer value
	 */
	Logger &operator<<(unsigned int t);
	
	/**
	 * @brief Write a signed long integer value to the logger
	 * 
	 * @param t signed long integer value
	 */
	Logger &operator<<(signed long t);
	
	/**
	 * @brief Write a unsigned long integer value to the logger
	 * 
	 * @param t unsigned long integer value
	 */
	Logger &operator<<(unsigned long t);
	
	/**
	 * @brief Write a qint64 value to the logger
	 * 
	 * @param t qint64 value
	 */
	Logger &operator<<(qint64 t);
	
	/**
	 * @brief Write a quint64 value to the logger
	 * 
	 * @param t quint64 value
	 */
	Logger &operator<<(quint64 t);
	
	/**
	 * @brief Write a float value to the logger
	 * 
	 * @param t float value
	 */
	Logger &operator<<(float t);
	
	/**
	 * @brief Write a double value to the logger
	 * 
	 * @param t double value
	 */
	Logger &operator<<(double t);
	
	/**
	 * @brief Write a const char* value to the logger
	 * 
	 * @param t const char* value
	 */
	Logger &operator<<(const char* t);
	
	/**
	 * @brief Write a QString value to the logger
	 * 
	 * @param t QString value
	 */
	Logger &operator<<(const QString & t);
	
	/**
	 * @brief Write a QStringRef value to the logger
	 * 
	 * @param t QStringRef value
	 */
	Logger &operator<<(const QStringRef & t);
	
	/**
	 * @brief Write a QLatin1String value to the logger
	 * 
	 * @param t QLatin1String value
	 */
	Logger &operator<<(const QLatin1String &t);
	
	/**
	 * @brief Write a QByteArray value to the logger
	 * 
	 * @param t QByteArray value
	 */
	Logger &operator<<(const QByteArray & t);
	
	/**
	 * @brief Write a const void * value to the logger
	 * 
	 * @param t const void * value
	 */
	Logger &operator<<(const void * t);
	
	/**
	 * @brief Write a Loggable object to the logger
	 * 
	 * @param t Loggable object
	 */
	Logger &operator<<(const Loggable & t);
	
	/**
	 * @brief Write a QTextStreamFunction value to the logger
	 * 
	 * @param f QTextStreamFunction value
	 */
	Logger &operator<<(QTextStreamFunction f);
	
	/**
	 * @brief Write a QTextStreamManipulator value to the logger
	 * 
	 * @param m QTextStreamManipulator value
	 */
	Logger &operator<<(QTextStreamManipulator m);
	
	/**
	 * @brief Write a Exception object to the logger
	 * 
	 * @param e Exception object
	 */
	Logger &operator<<(Exception &e);
	
	/**
	 * @brief Write a shared pointer object to the logger
	 * 
	 * @param p SharedPtr object
	 */
	template<typename T>
	Logger &operator<<(const SharedPtr<T> &p);
	
	/**
	 * @brief Write a HashMap object to the logger
	 * 
	 * @param p SharedPtr object
	 */
	Logger &operator<<(const QMap<QString, QVariant> &hash);
	
	/**
	 * @brief Get the global logger level
	 * 
	 * @return Level
	 */
	static Level level();
	
	/**
	 * @brief Set the global logger level
	 * 
	 * @param level Level
	 */
	static void setLevel(Level level);
	
	/**
	 * @brief Qt object dump
	 * 
	 * @return The human readable representation of the object
	 */
	template <typename T>
	static QString dump(const T& object);

};

inline void Logger::append(QString str) {
	m_stream << str.replace('\n', levelHdrs[m_level]);
}

inline void Logger::appendln() {
	m_stream << levelHdrs[m_level];
}

inline Logger::Logger(Logger::Level level)
	: m_stream(&m_buffer),
	  m_level(level) {
	m_stream << (levelHdrs[m_level] + 1);
}

inline Logger::Logger(const Logger &other)
	: m_buffer(other.m_buffer),
	  m_stream(&m_buffer),
	  m_level(other.m_level) {
	m_stream << (levelHdrs[m_level] + 1);
}

inline Logger::~Logger() {
	m_stream << levelFoot;
	std::cerr << m_buffer.toLocal8Bit().data() << std::endl;
}

inline Logger &Logger::operator<<(QChar t) {
	if (t == '\n') {
		appendln();
	} else {
		m_stream << t;
	}
	return *this;
}

inline Logger &Logger::operator<<(QBool t) {
	m_stream << (bool(t != 0) ? "true" : "false");
	return *this;
}

inline Logger &Logger::operator<<(bool t) {
	m_stream << (t ? "true" : "false");
	return *this;
}

inline Logger &Logger::operator<<(char t) {
	if (t == '\n') {
		appendln();
	} else {
		m_stream << t;
	}
	return *this;
}

inline Logger &Logger::operator<<(signed short t) {
	m_stream << t;
	return *this;
}

inline Logger &Logger::operator<<(unsigned short t) {
	m_stream << t;
	return *this;
}

inline Logger &Logger::operator<<(signed int t) {
	m_stream << t;
	return *this;
}

inline Logger &Logger::operator<<(unsigned int t) {
	m_stream << t;
	return *this;
}

inline Logger &Logger::operator<<(signed long t) {
	m_stream << t;
	return *this;
}

inline Logger &Logger::operator<<(unsigned long t) {
	m_stream << t;
	return *this;
}

inline Logger &Logger::operator<<(qint64 t) {
	m_stream << QString::number(t);
	return *this;
}

inline Logger &Logger::operator<<(quint64 t) {
	m_stream << QString::number(t);
	return *this;
}

inline Logger &Logger::operator<<(float t) {
	m_stream << t;
	return *this;
}

inline Logger &Logger::operator<<(double t) {
	m_stream << t;
	return *this;
}

inline Logger &Logger::operator<<(const char* t) {
	append(QString::fromAscii(t));
	return *this;
}

inline Logger &Logger::operator<<(const QString & t) {
	append(t);
	return *this;
}

inline Logger &Logger::operator<<(const QStringRef & t) {
	return operator<<(t.toString());
}

inline Logger &Logger::operator<<(const QLatin1String &t) {
	append(t.latin1());
	return *this;
}

inline Logger &Logger::operator<<(const void * t) {
	m_stream << t;
	return *this;
}

inline Logger &Logger::operator<<(const Loggable & t) {
	t.writeToLogger(*this);
	return *this;
}

inline Logger &Logger::operator<<(QTextStreamFunction f) {
	if (f == (QTextStreamFunction)endl) {
		appendln();
	} else {
		m_stream << f;
	}
	return *this;
}

inline Logger &Logger::operator<<(QTextStreamManipulator m) {
	m_stream << m;
	return *this;
}

template<typename T>
Logger &Logger::operator<<(const SharedPtr<T> &p) {
	m_stream << __demangle(typeid(p).name());
	if (p == nullptr) {
		m_stream << " [null]";
	} else {
		m_stream << " [0x" << 
		toHexString(p.data()) << "]:" << __demangle(typeid(*p).name()) << "*{" << p.data()->ref << '}';
	}
	return *this;
}

inline Logger &Logger::operator<<(const QMap<QString, QVariant> &hash) {
	unsigned int length = 0;
	for (auto it=hash.begin(); it != hash.end(); it++) {
		length = qMax<unsigned int>(length, it.key().length());
	}
	length++;
	for (auto it=hash.begin(); it != hash.end(); it++) {
		m_stream << levelHdrs[m_level] << "\t" << it.key().leftJustified(length, ' ') << ": " << it.value().toString();
	}
	return *this;
}

inline Logger::Level Logger::level() {
	return globalLevel;
}

template <typename T>
inline QString Logger::dump(const T& object) {
	QString buff;
	QDebug(&buff) << object;
	return buff;
}

inline void Logger::setLevel(Level level) {
	globalLevel = level;
}

inline Logger logCrit() {
	return Logger(Logger::CRITICAL);
}

inline Logger logWarn() {
	return Logger(Logger::WARNING);
}

inline Logger logInfo() {
	return Logger(Logger::INFO);
}

inline Logger logConf() {
	return Logger(Logger::CONFIG);
}

inline Logger logFine() {
	return Logger(Logger::FINE);
}

inline Logger logFiner() {
	return Logger(Logger::FINER);
}

inline Logger logFinest() {
	return Logger(Logger::FINEST);
}

}

#endif //JSONBUS_LOGGER_H
