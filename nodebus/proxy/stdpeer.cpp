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

#include "stdpeer.h"
#include "proxy.h"
#include "httppeer.h"
#include <typeinfo>
#include <nodebus/core/logger.h>
#include <nodebus/core/selectionkey.h>
#include <nodebus/core/streamchannel.h>
#include <nodebus/core/jsonparser.h>
#include <nodebus/core/jsonserializer.h>

QMap<QString, SharedPtr<StdPeer> > StdPeer::m_stdPeers;

void StdPeer::clearClientList() {
	m_stdPeers.clear();
}

SharedPtr< StdPeer > StdPeer::get(const QString& uid) {
	return m_stdPeers.value(uid);
}

StdPeer::StdPeer(SocketChannelPtr socket)
: Peer(socket), m_parser(socket), m_serializer(socket), m_synchronize(QMutex::Recursive) {
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
	logFiner() << "Peer << " << JSONSerializer::toString(res);
	m_serializer.serialize(res);
	m_socket->write("\n", 1);
}

void StdPeer::writeResponse(const QString &object, const QVariant &data) {
	QVariantMap res;
	res["type"] = QVariant("response");
	res["object"] = QVariant(object);
	res["status"] = QVariant("success");
	res["data"] = data;
	logFiner() << "Peer << " << JSONSerializer::toString(res);
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
	logFiner() << "Peer << " << JSONSerializer::toString(request);
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
		logFiner() << "Peer >> " << JSONSerializer::toString(message);
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
