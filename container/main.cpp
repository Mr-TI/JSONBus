
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include "container.h"

#include <signal.h>

using namespace JSONBus;

void onQuit(int signum) {
	qApp->quit();
}

int main(int argc, char **argv) {
	try {
		Container cnt(argc, argv);
		if (cnt.getCliArguments().isEnabled("help")) {
			cnt.getCliArguments().displayUseInstructions();
			return 0;
		}
		signal(SIGINT, onQuit);
		signal(SIGTERM, onQuit);
		cnt.launch();
	} catch (Exception &e) {
		outErr << "Exception: " << e.message() << endl;
	} catch (...) {
		outErr << ">>> Exception not managed !" << endl;
	}
	return 0;
}
