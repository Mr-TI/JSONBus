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

#include "httppeer.h"
#include "stdpeer.h"
#include "proxy.h"
#include <typeinfo>
#include <qt4/QtNetwork/QHttpHeader>
#include <qt4/QtCore/qshareddata.h>
#include <nodebus/core/logger.h>
#include <nodebus/core/selectionkey.h>
#include <nodebus/core/streamchannel.h>
#include <nodebus/core/parser.h>
#include <nodebus/core/serializer.h>

class HTTPExceptionData :public ExceptionData {
public:
	uint code;
	inline HTTPExceptionData(uint code, const QString& message): ExceptionData(message), code(code) {}
};

class  HTTPException :public Exception  {
public: 
	inline HTTPException (uint code, const QString &msg = ""):Exception (new HTTPExceptionData(code, msg)) {}
	inline virtual ~HTTPException () throw() {} 
	inline virtual void raise() const { throw *this; } 
	inline virtual HTTPException  *clone() const { return new HTTPException (*this); }
	inline uint code() { return data<HTTPExceptionData>()->code; }
};

HttpPeer::HttpPeer(SocketChannelPtr channel)
: Peer(channel), m_processDone(false) {
}

HttpPeer::~HttpPeer() {
	cancel();
}

void HttpPeer::cancel() {
	if (m_socket == nullptr) {
		return;
	}
	if (m_stdPeer != NULL) {
		m_stdPeer->rmRef(m_reqUid);
	}
	Peer::cancel();
}

void HttpPeer::sendResult(uint code, const QVariant& content) {
	QByteArray msgData;
	Serializer(msgData).serialize(content);
	QHttpResponseHeader rspHdr;
	rspHdr.setStatusLine(code);
	rspHdr.setContentLength(msgData.length());
	rspHdr.setContentType("application/json");
	logFiner() << rspHdr.toString() << QString(msgData);
	QByteArray hdrData = rspHdr.toString().toUtf8();
	m_socket->write(hdrData.constData(), hdrData.length());
	m_socket->write(msgData.constData(), msgData.length());
	m_socket->close();
}

void HttpPeer::process() {
	size_t n;
	if (m_processDone) {
		while ((n = m_socket->available()) > 0) {
			m_socket->ignore(n);
		}
		Proxy::getInstance().getPeerAdmin().attach(m_socket, this);
		return;
	}
	m_socket->setDeadLine(QDateTime::currentMSecsSinceEpoch() + 30000);
	try {
		QString data;
		try {
			char c1 = m_socket->get(),c2 = m_socket->get(),c3 = m_socket->get(),c4 = m_socket->get();
			data.append(c1);
			data.append(c2);
			while (c1 != '\r' ||  c3 != '\r' || c2 != '\n' || c4 != '\n') {
				data.append(c3);
				c1 = c2;
				c2 = c3;
				c3 = c4;
				c4 = m_socket->get();
			}
		} catch (IOTimeoutException &e) {
			throw HTTPException(408, "Request timeout");
		}
		logFiner() << data;
		QHttpRequestHeader httpHeader(data);
		if (!httpHeader.isValid()) {
			throw HTTPException(400, "Invalid HTTP header");
		}
		if (httpHeader.method() != "POST") {
			throw HTTPException(405, "Only POST method is supported");
		}
		if (httpHeader.contentType() != "application/json") {
			throw HTTPException(415, "Content type must be 'application/json'");
		}
		uint payloadLen = httpHeader.contentLength();
		if (payloadLen == 0) {
			throw HTTPException(400, "No data found");
		}
		char payloadData[payloadLen + 1];
		payloadData[payloadLen] = '\0';
		try {
			uint n = 0;
			while (n < payloadLen) {
				n += m_socket->read(payloadData + n, payloadLen - n);
			}
		} catch (IOTimeoutException &e) {
			throw HTTPException(408, "Request timeout");
		}
		logFiner() << payloadData;
		while ((n = m_socket->available()) > 0) {
			m_socket->ignore(n);
		}
		QVariantMap message;
		try {
			message = Parser::parse(payloadData, payloadLen).toMap();
		} catch (Exception &e) {
			throw HTTPException(400, "Data parse error: " + e.message());
		}
		QString uid = httpHeader.path().section('/', 1);
		if (!uid.isEmpty()) {
			m_stdPeer = StdPeer::get(uid);
			if (m_stdPeer == nullptr) {
				throw HTTPException(404, "There is no box with the given uid '" + uid + "'");
			}
		}
		QString object = message["object"].toString();
		if (object.isEmpty()) {
			throw HTTPException(400, "Malformed message, missing 'object' field");
		}
		QString type = message["type"].toString();
		if (m_stdPeer != nullptr) {
			if (type != "request") {
				throw HTTPException(400, "Only 'request' messge type can be forwarded");
			}
			m_reqUid = m_stdPeer->send(message, this);
			Proxy::getInstance().getPeerAdmin().attach(m_socket, this);
			m_processDone = true;
			return;
		}
		if (object != "Proxy") {
			throw HTTPException(400, "Service '" + object + "' not found");
		}
		QString method = message["method"].toString();
		QVariant variant;
		if (method == "ping") {
		} else if (method == "help") {
		} else if (method == "getStatus") {
			QVariantMap res;
			res["box-count"] = StdPeer::getCount();
			variant = res;
		} else if (method == "getBoxList") {
			QList<QString> list = StdPeer::getUidList();
			QVariantList res;
			for (auto it = list.begin(); it != list.end(); it++) {
				res.append(*it);
			}
			variant = res;
		} else {
			throw HTTPException(400, "invalid method '" + method + "'");
		}
		QVariantMap res;
		res["type"] = "response";
		res["status"] = "success";
		res["data"] = variant;
		sendResult(200, res);
	} catch (HTTPException &e) {
		logConf() << "HTTP " << e.code() << ": " << e.message();
		QVariantMap res;
		res["type"] = "response";
		res["object"] = "Proxy";
		res["status"] = "failure";
		res["error-message"] = e.message();
		sendResult(e.code(), res);
	}
}
