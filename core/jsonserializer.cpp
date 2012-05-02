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

#include <common.h>
#include <jsonserializer.h>

#define JSON_NULL "null"
#define JSON_TRUE "true"
#define JSON_FALSE "false"
#define JSON_OBJECT_BEGIN "{"
#define JSON_OBJECT_END "}"
#define JSON_MEMBER_SEP ":"
#define JSON_ELEMENT_SEP ","
#define JSON_ARRAY_BEGIN "["
#define JSON_ARRAY_END "]"

namespace JSONBus {

static QString sanitizeString( QString str )
{
  str.replace( QLatin1String( "\\" ), QLatin1String( "\\\\" ) );
  str.replace( QLatin1String( "\"" ), QLatin1String( "\\\"" ) );
  str.replace( QLatin1String( "\b" ), QLatin1String( "\\b" ) );
  str.replace( QLatin1String( "\f" ), QLatin1String( "\\f" ) );
  str.replace( QLatin1String( "\n" ), QLatin1String( "\\n" ) );
  str.replace( QLatin1String( "\r" ), QLatin1String( "\\r" ) );
  str.replace( QLatin1String( "\t" ), QLatin1String( "\\t" ) );
  return QString( QLatin1String( "\"%1\"" ) ).arg( str );
}

JSONSerializer::JSONSerializer(QObject *parent)
: QObject(parent), m_stream(*new StdStream(cout)) {
	
}

JSONSerializer::JSONSerializer(ostream& stream, QObject* parent)
: QObject(parent), m_stream(*new StdStream(stream)) {
	
}

JSONSerializer::JSONSerializer(const JSONBus::JSONSerializer::Stream& stream, QObject* parent)
: QObject(parent), m_stream(*stream.clone()) {
	
}

JSONSerializer::~JSONSerializer() {
	delete &m_stream;
}

void JSONSerializer::serialize(const QVariant &variant) {
	if (!variant.isValid()) { // Case of JSON null/undefined
		// TODO:find a way to defferenciate null/undefined
		m_stream << JSON_NULL;
	} else if (variant.type() == QVariant::Bool) { // Case of JSON boolean
		m_stream << (variant.toBool() ? JSON_TRUE: JSON_FALSE);
	} else if (variant.type() == QVariant::Map) { // Case of JSON object
		m_stream << JSON_OBJECT_BEGIN;
		const QVariantMap elements = variant.toMap();
		auto it = elements.begin();
		if (it != elements.end()) {
			m_stream << sanitizeString(it.key()) << JSON_MEMBER_SEP;
			serialize(it.value());
			it++;
		}
		while (it != elements.end()) {
			m_stream << JSON_ELEMENT_SEP << sanitizeString(it.key()) << JSON_MEMBER_SEP;
			serialize(it.value());
			it++;
		}
		m_stream << JSON_OBJECT_END;
	} else if (variant.type() == QVariant::List) { // Case of JSON array
		m_stream << JSON_ARRAY_BEGIN;
		const QVariantList elements = variant.toList();
		auto it = elements.begin();
		if (it != elements.end()) {
			serialize(*it);
			it++;
		}
		while (it != elements.end()) {
			m_stream << JSON_MEMBER_SEP;
			serialize(*it);
			it++;
		}
		m_stream << JSON_ARRAY_END;
	} else if ((variant.type() == QVariant::String) 
		|| (variant.type() == QVariant::ByteArray)) { // Case of JSON string
		m_stream << sanitizeString(variant.toString());
	} else if ((variant.type() == QVariant::Double) || variant.canConvert<double>()) {
		m_stream << QString::number(variant.toDouble()).replace("inf", "infinity");
	} else if ((variant.type() == QVariant::String) || variant.canConvert<QString>()) {
		m_stream << variant.toString();
	} else {
		throw JSONSerializerException("Fatal: QVariant type not managed.");
	}
}

}
