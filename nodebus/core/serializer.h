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
 * @brief NodeBus : BSON serializer management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */

#ifndef NODEBUS_SERIALIZER_H
#define NODEBUS_SERIALIZER_H

#include <nodebus/core/exception.h>
#include <nodebus/core/global.h>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#include <nodebus/core/datastream.h>
#include <QString>
#include <QVariant>

namespace NodeBus {

nodebus_declare_exception(SerializerException, Exception);

/**
 * @brief BSON serializer management.
 */
class NODEBUS_EXPORT Serializer {
public:
	
	static uint32_t FORMAT_COMPACT;
	static uint8_t INDENT(uint8_t size);
	
	/**
	 * @brief Serializer constructor.
	 * @param stream A reference to the output stream
	 */
	Serializer(DataStream &dataStream, FileFormat format=JSON);
	
	/**
	 * @brief Serializer destructor.
	 */
	~Serializer();
	
	/**
	 * @brief Serialize an object in BSON format
	 * @param variant object to serialize
	 */
	void serialize(const QVariant &variant, uint32_t flags = FORMAT_COMPACT);
	
	static void serialize(DataStream &dataStream, const QVariant &variant, FileFormat format=JSON, uint32_t flags = FORMAT_COMPACT);
	static void toFile(const QString &fileName, const QVariant &variant, FileFormat format=JSON, uint32_t flags = FORMAT_COMPACT);
	
	/**
	 * @brief Serialize an object in BSON format
	 * @param variant object to serialize
	 */
	static QString toJSONString(const QVariant &variant, uint32_t flags);
private:
	void serializeBCON(const QVariant &variant, const QString *key=NULL);
	void serializeJSON(const QVariant &variant, uint32_t flags);
	QByteArray serializeBSONDocument(const QVariant &variant);
	QByteArray serializeBSONElt(const QVariant& variant, const QString &key);
	template <typename T> void write(quint8 type, T value);
	template <typename T> void write(QByteArray& output, T value);
	void write64(QByteArray& output, uint64_t value);
	DataStream &m_dataStream;
	FileFormat m_format;
};

inline uint8_t Serializer::INDENT(uint8_t size) {
	return size & 0x1Fu;
}

}

#endif
