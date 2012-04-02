
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <master.h>

using namespace JSONBus;

int main(int argc, char **argv) {
	try {
		Master service(argc, argv);
		if (service.getCliArguments().isEnabled("help")) {
			service.getCliArguments().displayUseInstructions();
			return 0;
		}
#ifdef WIN32
		if (service.getCliArguments().isEnabled("win32-cli")) {
			SERVICE_TABLE_ENTRY Table[] = {{"JSONBus Service", Master::launchInstance}, {NULL, NULL}};
			if (!StartServiceCtrlDispatcher(Table)) {
				clog << getMessageError(GetLastError());
			}
		} else {
			service.launch();
		}
#else
		service.launch();
#endif
	} catch (Exception e) {
		qCritical() << "Exception: " << e.message() << "\n";
	} catch (...) {
		qCritical() << "### WARNING ! WARNING ! WARNING ! ### Exception not managed !\n";
	}
	return 0;
}
