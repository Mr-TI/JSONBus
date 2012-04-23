
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>

using namespace JSONBus;
using namespace std;



int main(int argc, char **argv) {
// 	QCoreApplication app(argc, argv);
	
	QTextStream sin(fopen("/tmp/test", "r"));
	
	qlonglong test;
	do {
		sin >> test;
		qDebug() << test;
	} while (1);
	
	return 0;
}
