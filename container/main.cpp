
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include "container.h"

using namespace JSONBus;

int main(int argc, char **argv) {
	try {
		Container cnt(argc, argv);
		if (cnt.getCliArguments().isEnabled("help")) {
			cnt.getCliArguments().displayUseInstructions();
			return 0;
		}
		cnt.launch();
	} catch (Exception e) {
		outErr << e.getMsg() << "\n" << endl;
	} catch (...) {
		outErr << "### WARNING ! WARNING ! WARNING ! ### Exception not managed !\n" << endl;
	}
	return 0;
}
