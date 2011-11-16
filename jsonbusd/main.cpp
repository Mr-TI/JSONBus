
#include <jsonbus/core/common.h>
#include <jsonbus/core/cliarguments.h>
#include <masterservice.h>

using namespace JSONBus;

int main(int argc, char **argv) {
	try {
		MasterService service(argc, argv);
		if (service.getCliArguments().isEnabled("help")) {
			service.getCliArguments().displayUseInstructions();
			return 0;
		}
#ifdef WIN32
		if (service.getCliArguments().isEnabled("win32-cli")) {
			SERVICE_TABLE_ENTRY Table[] = {{"JSONBus Service", MasterService::launchInstance}, {NULL, NULL}};
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
		outErr << e.getMsg() << "\n" << endl;
	} catch (...) {
		outErr << "### WARNING ! WARNING ! WARNING ! ### Exception not managed !\n" << endl;
	}
	return 0;
}
