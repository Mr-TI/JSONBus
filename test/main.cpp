
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/argsparser.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>

using namespace jsonbus;
using namespace std;



int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    ArgsParser args;
    args.define("daeminize", 'd', "Launch the service in background.");
    args.define("config", 'c', "Set a custom config path.", "/etc/json/jsond.conf");
    args.define("help", 'h', "Display this message.");
    args.define("version", 0, "Display the version.");
    args.displayUseInstructions();
    //fillArgs(args, QCoreApplication::arguments());
    return 0;
}
