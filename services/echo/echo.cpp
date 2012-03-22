
#include <QStringList>
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <jsonbus/core/settings.h>

#include "echo.h"

jsonbus_declare_plugin(Echo)

Echo::Echo() {
	cerr << "Creating echo service..." << endl;
}

Echo::~Echo() {
	cerr << "Destroying echo service..." << endl;
}

void Echo::onInit (Settings &settings) {
	cerr << "Initializing echo service..." << endl;
};

void Echo::onLoad () {
	cerr << "Loading echo service..." << endl;
	Plugin::onLoad();
};

void Echo::onUnload () {
	cerr << "Unloading echo service..." << endl;
	Plugin::onUnload();
};
