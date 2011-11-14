
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/cliarguments.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>

using namespace jsonbus;
using namespace std;



int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    CliArguments args;
    args.define("daeminize", 'd', "Launch the service in background.");
    args.define("config", 'c', "Set a custom config path.", "/etc/json/jsond.conf");
    args.define("help", 'h', "Display this message.");
    args.define("version", 0, "Display the version.");
    try {
      args.parse(QCoreApplication::arguments());
      if (args.isEnabled("help")) {
        args.displayUseInstructions();
      }
    } catch (Exception e) {
      cout << "Exception: " << e.getMessage() << endl;
    } catch (...) {
      cout << "Unknow exception." << endl;
    }
    return 0;
}
