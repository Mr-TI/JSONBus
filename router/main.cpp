
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include "router.h"

using namespace JSONBus;

int main(int argc, char **argv) {
	try {
		Router router(argc, argv);
		if (router.getCliArguments().isEnabled("help")) {
			router.getCliArguments().displayUseInstructions();
			return 0;
		}
		router.launch();
	} catch (Exception e) {
		outErr << e.getMsg() << "\n" << endl;
	} catch (...) {
		outErr << "### WARNING ! WARNING ! WARNING ! ### Exception not managed !\n" << endl;
	}
	return 0;
}
