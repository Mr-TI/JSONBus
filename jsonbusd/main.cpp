
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>
#include <masterservice.h>

using namespace jsonbus;

int main(int argc, char **argv) {
  MasterService service(argc, argv);
  try {
#ifdef WIN32
    int i = 1;
    while (i < argc) { //Boucle de récupération des arguments
      if (strcmp(argv[i], "-m") == 0) { //Mode service windows désactivé
        MasterService::isService = false;
      }
      i++;
    }
    if (MasterService::isService) {
      SERVICE_TABLE_ENTRY Table[] = {{"JSONBus Service", MasterService::launchInstance}, {NULL, NULL}};
      if (!StartServiceCtrlDispatcher(Table)) {
        FILE *ffile = fopen("C:\\jsonbus_service_fatal.log", "a");
        fprintf(ffile, "StartServiceCtrlDispatcher() >> %s\n", getMessageError(GetLastError()).toAscii());
        fclose(ffile);
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
