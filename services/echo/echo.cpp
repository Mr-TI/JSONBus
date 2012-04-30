
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>

#include "echo.h"

jsonbus_declare_plugin(Echo)

Echo::Echo() {
// 	qDebug() << "Creating echo service...";
}

Echo::~Echo() {
// 	qDebug() << "Destroying echo service...";
}

void Echo::onInit(Settings &settings) {
// 	qDebug() << "Initializing echo service...";
	settings.define("fieldName", "The field name", QVariant("name"));
	settings.define("helloMsg", "Hello message", QVariant("Hello"));
};

void Echo::onLoad(Settings &settings) {
	Plugin::onLoad(settings);
// 	qDebug() << "Loading echo service...";
	m_fieldName = settings.value("fieldName", "name").toString();
	m_helloMsg = settings.value("helloMsg", "Hello").toString();
};

void Echo::onUnload() {
// 	qDebug() << "Unloading echo service...";
	Plugin::onUnload();
};

void Echo::onRequest(QVariant request) {
// 	qDebug() << "Request: " << request;
	QVariant result;
	QVariantMap m;
	m["reply"].setValue(m_helloMsg + " " + request.toMap()[m_fieldName].toString());
	result.setValue(m);
	emit resultAvailable(result);
}

