
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>

#include "echo.h"

jsonbus_declare_plugin(Echo)

Echo::Echo() {
	outErr << "Creating echo service..." << endl;
}

Echo::~Echo() {
	outErr << "Destroying echo service..." << endl;
}

void Echo::onInit(Settings &settings) {
	outErr << "Initializing echo service..." << endl;
};

void Echo::onLoad() {
	outErr << "Loading echo service..." << endl;
	Plugin::onLoad();
};

void Echo::onUnload() {
	outErr << "Unloading echo service..." << endl;
	Plugin::onUnload();
};

void Echo::onRequest(QVariant request) {
	outErr << "Request" << endl;
}

