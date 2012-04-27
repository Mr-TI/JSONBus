
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include "container.h"

#include <signal.h>
#include <fcntl.h>

using namespace JSONBus;

void onQuit(int signum) {
	qApp->quit();
}

int main(int argc, char **argv) {
	Container cnt(argc, argv);
	if (cnt.getCliArguments().isEnabled("help")) {
		cnt.getCliArguments().displayUseInstructions();
		return 0;
	}
	signal(SIGINT, onQuit);
	signal(SIGTERM, onQuit);
	cnt.launch();
	return 0;
}
