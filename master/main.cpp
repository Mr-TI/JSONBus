
#include <jsonbus/core/common.h>
#include <master.h>

using namespace JSONBus;

int main(int argc, char **argv) {
	try {
		Master service(argc, argv);
		CliArguments &args = CliArguments::getInstance();
		if (args.isEnabled("help")) {
			args.displayUseInstructions();
			return 0;
		}
#ifdef WIN32
		if (args.isEnabled("win32-cli")) {
			SERVICE_TABLE_ENTRY Table[] = {{"JSONBus Service", Master::runInstance}, {NULL, NULL}};
			if (!StartServiceCtrlDispatcher(Table)) {
				clog << getMessageError(GetLastError());
			}
		} else {
			service.run();
		}
#else
		service.run();
#endif
	} catch (Exception e) {
		qCritical() << "Exception: " << e.message() << "\n";
	} catch (...) {
		qCritical() << "### WARNING ! WARNING ! WARNING ! ### Exception not managed !\n";
	}
	return 0;
}
