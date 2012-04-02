
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>

#include "echo.h"

jsonbus_declare_plugin(Echo)

Echo::Echo() {
	qDebug() << "Creating echo service...";
}

Echo::~Echo() {
	qDebug() << "Destroying echo service...";
}

void Echo::onInit(Settings &settings) {
	qDebug() << "Initializing echo service...";
};

void Echo::onLoad() {
	qDebug() << "Loading echo service...";
	Plugin::onLoad();
};

void Echo::onUnload() {
	qDebug() << "Unloading echo service...";
	Plugin::onUnload();
};

void Echo::onRequest(QVariant request) {
	qDebug() << "Request";
}

