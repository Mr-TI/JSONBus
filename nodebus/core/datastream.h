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
 * @brief NodeBus : Data stream
 *
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */

#ifndef NODEBUS_INPUTSTREAM_H
#define NODEBUS_INPUTSTREAM_H

#include <nodebus/core/exception.h>
#include <nodebus/core/global.h>
#include "logger.h"

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#include <QString>
#include <QVariant>

namespace NodeBus {

/**
 * @brief Data stream
 */
class NODEBUS_EXPORT DataStream {
public:

	/**
	 * @brief DataStream constructor.
	 */
	DataStream(QIODevice *device);

	/**
	 * @brief DataStream destructor.
	 */
	virtual ~DataStream();

	DataStream &operator>>(char &i);
	DataStream &operator>>(qint8 &i);
	DataStream &operator>>(quint8 &i);
	DataStream &operator>>(qint16 &i);
	DataStream &operator>>(quint16 &i);
	DataStream &operator>>(qint32 &i);
	DataStream &operator>>(quint32 &i);
	DataStream &operator>>(qint64 &i);
	DataStream &operator>>(quint64 &i);
	DataStream &operator>>(float &f);
	DataStream &operator>>(double &f);

	DataStream &operator<<(char i);
	DataStream &operator<<(qint8 i);
	DataStream &operator<<(quint8 i);
	DataStream &operator<<(qint16 i);
	DataStream &operator<<(quint16 i);
	DataStream &operator<<(qint32 i);
	DataStream &operator<<(quint32 i);
	DataStream &operator<<(qint64 i);
	DataStream &operator<<(quint64 i);
	DataStream &operator<<(float f);
	DataStream &operator<<(double f);
// 	DataStream &operator<<(const char *str);
	DataStream &operator<<(const QString &byteArray);
	DataStream &operator<<(const QByteArray &byteArray);

	virtual quint64 read(char *buf, quint32 len, bool full = true);
	virtual quint64 write(const char *buf, quint32 len, bool full = true);
private:
	DataStream(const DataStream&);
	DataStream& operator =(const DataStream&);
	bool m_noswap;
	QIODevice *m_device;
};

inline DataStream &DataStream::operator>>(char &i) {
	return *this >> reinterpret_cast<qint8&>(i);
}

inline DataStream &DataStream::operator>>(quint8 &i) {
	return *this >> reinterpret_cast<qint8&>(i);
}

inline DataStream &DataStream::operator>>(quint16 &i) {
	return *this >> reinterpret_cast<qint16&>(i);
}

inline DataStream &DataStream::operator>>(quint32 &i) {
	return *this >> reinterpret_cast<qint32&>(i);
}

inline DataStream &DataStream::operator>>(quint64 &i) {
	return *this >> reinterpret_cast<qint64&>(i);
}

inline DataStream &DataStream::operator<<(char i) {
	return *this << qint8(i);
}

inline DataStream &DataStream::operator<<(quint8 i) {
	return *this << qint8(i);
}

inline DataStream &DataStream::operator<<(quint16 i) {
	return *this << qint16(i);
}

inline DataStream &DataStream::operator<<(quint32 i) {
	return *this << qint32(i);
}

inline DataStream &DataStream::operator<<(quint64 i) {
	return *this << qint64(i);
}

inline quint64 DataStream::read(char* buf, quint32 len, bool full) {
	return m_device->read(buf, len);
}

inline quint64 DataStream::write(const char* buf, quint32 len, bool full) {
	logFinest() << QByteArray(buf, len);
	return m_device->write(buf, len);
}

inline DataStream& DataStream::operator<<(const QString& str) {
	return operator<<(str.toLocal8Bit());
}

}

#endif
