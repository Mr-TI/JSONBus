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

#include "stdpeer.h"
#include "proxy.h"
#include "httppeer.h"
#include <typeinfo>
#include <nodebus/core/logger.h>
#include <nodebus/nio/selectionkey.h>
#include <nodebus/nio/streamchannel.h>
#include <nodebus/core/parser.h>
#include <nodebus/core/serializer.h>

QMap<QString, SharedPtr<StdPeer> > StdPeer::m_stdPeers;

void StdPeer::clearClientList() {
	m_stdPeers.clear();
}

SharedPtr< StdPeer > StdPeer::get(const QString& uid) {
	return m_stdPeers.value(uid);
}

StdPeer::StdPeer(SocketChannelPtr socket, FileFormat format)
: Peer(socket), m_dataStream(socket.data()), m_parser(m_dataStream, format), m_serializer(m_dataStream, format), m_synchronize(QMutex::Recursive) {
}

StdPeer::~StdPeer() {
	cancel();
}

void StdPeer::cancel() {
	QMutexLocker locker(&m_synchronize);
	if (m_socket == nullptr) {
		return;
	}
	if (!m_uid.isEmpty()) {
		m_stdPeers.remove(m_uid);
		logInfo() << "Peer[" << m_uid << "] unregistred";
		m_uid.clear();
	}
	Peer::cancel();
}

void StdPeer::writeError(const QString &object, const QString &message, const QString &type) {
	QVariantMap res;
	res["type"] = QVariant(type);
	res["object"] = QVariant(object);
	res["status"] = QVariant("failure");
	res["error-message"] = QVariant(message);
	logFiner() << "Peer << " << Serializer::toJSONString(res, Serializer::INDENT(2));
	m_serializer.serialize(res);
	m_socket->write("\n", 1);
}

void StdPeer::writeResponse(const QString &object, const QVariant &data) {
	QVariantMap res;
	res["type"] = QVariant("response");
	res["object"] = QVariant(object);
	res["status"] = QVariant("success");
	res["data"] = data;
	logFiner() << "Peer << " << Serializer::toJSONString(res, Serializer::INDENT(2));
	m_serializer.serialize(res);
	m_socket->write("\n", 1);
}

QString StdPeer::send(QVariantMap &request, SharedPtr<HttpPeer> peer) {
	QMutexLocker locker(&m_synchronize);
	QString uid;
	if (request.contains("uid")) {
		uid = request["uid"].toString();
	} else {
		uid = QUuid::createUuid().toString();
		request["uid"] = uid;
	}
	m_httpPeers[uid] = peer;
	logFiner() << "Peer << " << Serializer::toJSONString(request, Serializer::INDENT(2));
	m_serializer.serialize(request);
	m_socket->write("\n", 1);
	return uid;
}

void StdPeer::rmRef(const QString& reqUid) {
	m_httpPeers.remove(reqUid);
}

void StdPeer::process() {
	while (true) {
		QMutexLocker locker(&m_synchronize);
		while (m_socket->available() > 0 && isspace(m_socket->peek())) {
			m_socket->get();
		}
		if (m_socket->available() == 0) {
			Proxy::getInstance().getPeerAdmin().attach(m_socket, this);
			return;
		}
		m_socket->setDeadLine(QDateTime::currentMSecsSinceEpoch() + 30000);
		QVariantMap message = m_parser.parse().toMap();
		logFiner() << "Peer >> " << Serializer::toJSONString(message, Serializer::INDENT(2));
		QString object = message["object"].toString();
		if (object.isEmpty()) {
			writeError("Proxy", "Malformed message, missing 'object' field");
			break;
		}
		QString type = message["type"].toString();
		if (type.isEmpty() || type == "message") {
			break;
		}
		if (type == "request") {
			QString method = message["method"].toString();
			QVariantMap parameters = message["parameters"].toMap();
			if (object != "Proxy" && object != "Gateway") {
				writeError("Proxy", "Service '" + object + "' not found", "response");
				break;
			}
			if (method == "register") {
				if (!m_uid.isEmpty()) {
					writeError("Proxy", "Already registred", "response");
					break;
				}
				m_uid = parameters["uid"].toString();
				if (m_uid.isEmpty()) {
					m_uid = parameters["tuid"].toString();
				}
				if (m_uid.isEmpty()) {
					writeError("Proxy", "register: Missing parameter 'uid'", "response");
					break;
				}
				m_stdPeers[m_uid] = this;
				logInfo() << "Peer[" << m_uid << "] registred";
				writeResponse("Proxy", QVariant());
			} else if (method == "help") {
				
			} else {
				writeError("Proxy", "invalid '" + method + "' method", "response");
			}
		} else if (type == "response") {
			QString status = message["status"].toString();
			if (status == "success" || status == "failure") {
				QString msguid = message["rel-msg-uid"].toString();
				SharedPtr<HttpPeer> client = m_httpPeers.value(msguid);
				if (client != nullptr) {
					m_httpPeers.remove(msguid);
					client->sendResult(200, message);
				}
			}
		} else {
			writeError("Proxy", "Malformed message, invalid 'type' field");
		}
	}
}
