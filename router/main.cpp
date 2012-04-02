
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include "router.h"

using namespace JSONBus;

int main(int argc, char **argv) {
	Router router(argc, argv);
	if (router.getCliArguments().isEnabled("help")) {
		router.getCliArguments().displayUseInstructions();
		return 0;
	}
	router.launch();
	return 0;
}
