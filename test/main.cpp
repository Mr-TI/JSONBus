
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>

using namespace jsonbus;
using namespace std;



int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QMap<QString, QString> args;
    //fillArgs(args, QCoreApplication::arguments());
    return 0;
}
