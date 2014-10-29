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

#include "common.h"
#include "datastream.h"
#include <QVariant>

namespace NodeBus {

DataStream::DataStream(QIODevice *device)
	: m_noswap(QSysInfo::ByteOrder == QSysInfo::LittleEndian), m_device(device) {
}

DataStream::~DataStream() {

}

DataStream &DataStream::operator>>(qint8 &i) {
	i = 0;
	char c;
	read(&c, 1);
	i = qint8(c);
	return *this;
}

DataStream &DataStream::operator>>(qint16 &i) {
	i = 0;
	if (m_noswap) {
		read((char *)&i, 2);
	} else {
		register uchar *p = (uchar *)(&i);
		char b[2];
		read(b, 2);
		*p++ = b[1];
		*p = b[0];
	}
	return *this;
}

DataStream &DataStream::operator>>(qint32 &i) {
	i = 0;
	if (m_noswap) {
		read((char *)&i, 4);
	} else {                                        // swap bytes
		uchar *p = (uchar *)(&i);
		char b[4];
		read(b, 4);
		*p++ = b[3];
		*p++ = b[2];
		*p++ = b[1];
		*p   = b[0];
	}
	return *this;
}

DataStream &DataStream::operator>>(qint64 &i) {
	i = qint64(0);
	if (m_noswap) {                        // no conversion needed
		read((char *)&i, 8);
	} else {                                        // swap bytes
		uchar *p = (uchar *)(&i);
		char b[8];
		read(b, 8);
		*p++ = b[7];
		*p++ = b[6];
		*p++ = b[5];
		*p++ = b[4];
		*p++ = b[3];
		*p++ = b[2];
		*p++ = b[1];
		*p   = b[0];
	}
	return *this;
}

DataStream &DataStream::operator>>(float &f) {
	f = 0.0f;
	if (m_noswap) {
		read((char *)&f, 4);
	} else {                                        // swap bytes
		uchar *p = (uchar *)(&f);
		char b[4];
		read(b, 4);
		*p++ = b[3];
		*p++ = b[2];
		*p++ = b[1];
		*p = b[0];
	}
	return *this;
}

#if defined(Q_DOUBLE_FORMAT)
#define Q_DF(x) Q_DOUBLE_FORMAT[(x)] - '0'
#endif

DataStream &DataStream::operator>>(double &f) {
	f = 0.0;
#ifndef Q_DOUBLE_FORMAT
	if (m_noswap) {
		read((char *)&f, 8);
	} else {                                        // swap bytes
		register uchar *p = (uchar *)(&f);
		char b[8];
		read(b, 8);
		*p++ = b[7];
		*p++ = b[6];
		*p++ = b[5];
		*p++ = b[4];
		*p++ = b[3];
		*p++ = b[2];
		*p++ = b[1];
		*p   = b[0];
	}
#else
	//non-standard floating point format
	register uchar *p = (uchar *)(&f);
	char b[8];
	read(b, 8);
	if (m_noswap) {
		*p++ = b[Q_DF(0)];
		*p++ = b[Q_DF(1)];
		*p++ = b[Q_DF(2)];
		*p++ = b[Q_DF(3)];
		*p++ = b[Q_DF(4)];
		*p++ = b[Q_DF(5)];
		*p++ = b[Q_DF(6)];
		*p = b[Q_DF(7)];
	} else {
		*p++ = b[Q_DF(7)];
		*p++ = b[Q_DF(6)];
		*p++ = b[Q_DF(5)];
		*p++ = b[Q_DF(4)];
		*p++ = b[Q_DF(3)];
		*p++ = b[Q_DF(2)];
		*p++ = b[Q_DF(1)];
		*p = b[Q_DF(0)];
	}
#endif
	return *this;
}

DataStream &DataStream::operator<<(qint8 i) {
	write((char*)&i, 1);
	return *this;
}

DataStream &DataStream::operator<<(qint16 i) {
	if (m_noswap) {
		write((char *)&i, sizeof(qint16));
	} else {                                        // swap bytes
		register uchar *p = (uchar *)(&i);
		char b[2];
		b[1] = *p++;
		b[0] = *p;
		write(b, 2);
	}
	return *this;
}

DataStream &DataStream::operator<<(qint32 i) {
	if (m_noswap) {
		write((char *)&i, sizeof(qint32));
	} else {                                        // swap bytes
		register uchar *p = (uchar *)(&i);
		char b[4];
		b[3] = *p++;
		b[2] = *p++;
		b[1] = *p++;
		b[0] = *p;
		write(b, 4);
	}
	return *this;
}

DataStream &DataStream::operator<<(qint64 i) {
	if (m_noswap) {                        // no conversion needed
		write((char *)&i, sizeof(qint64));
	} else {                                        // swap bytes
		register uchar *p = (uchar *)(&i);
		char b[8];
		b[7] = *p++;
		b[6] = *p++;
		b[5] = *p++;
		b[4] = *p++;
		b[3] = *p++;
		b[2] = *p++;
		b[1] = *p++;
		b[0] = *p;
		write(b, 8);
	}
	return *this;
}

DataStream &DataStream::operator<<(float f) {
	float g = f;                                // fixes float-on-stack problem
	if (m_noswap) {                                // no conversion needed
		write((char *)&g, sizeof(float));
	} else {                                // swap bytes
		register uchar *p = (uchar *)(&g);
		char b[4];
		b[3] = *p++;
		b[2] = *p++;
		b[1] = *p++;
		b[0] = *p;
		write(b, 4);
	}
	return *this;
}

DataStream &DataStream::operator<<(double f) {
#ifndef Q_DOUBLE_FORMAT
	if (m_noswap) {
		write((char *)&f, sizeof(double));
	} else {
		register uchar *p = (uchar *)(&f);
		char b[8];
		b[7] = *p++;
		b[6] = *p++;
		b[5] = *p++;
		b[4] = *p++;
		b[3] = *p++;
		b[2] = *p++;
		b[1] = *p++;
		b[0] = *p;
		write(b, 8);
	}
#else
	register uchar *p = (uchar *)(&f);
	char b[8];
	if (m_noswap) {
		b[Q_DF(0)] = *p++;
		b[Q_DF(1)] = *p++;
		b[Q_DF(2)] = *p++;
		b[Q_DF(3)] = *p++;
		b[Q_DF(4)] = *p++;
		b[Q_DF(5)] = *p++;
		b[Q_DF(6)] = *p++;
		b[Q_DF(7)] = *p;
	} else {
		b[Q_DF(7)] = *p++;
		b[Q_DF(6)] = *p++;
		b[Q_DF(5)] = *p++;
		b[Q_DF(4)] = *p++;
		b[Q_DF(3)] = *p++;
		b[Q_DF(2)] = *p++;
		b[Q_DF(1)] = *p++;
		b[Q_DF(0)] = *p;
	}
	write(b, 8);
#endif
	return *this;
}

// DataStream& DataStream::operator<<(const char* str) {
// 	write(str, strlen(str));
// 	return *this;
// }

DataStream& DataStream::operator<<(const QByteArray& byteArray) {
	write(byteArray.constData(), byteArray.length());
	return *this;
}

}
