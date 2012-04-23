
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>

using namespace JSONBus;
using namespace std;



int main(int argc, char **argv) {
// 	QCoreApplication app(argc, argv);
	
	QVariant val("-infinity");
	
	qreal r = val.toReal();

// 	qreal r = -1502145000000000000;
// 	
// 	QVariant v(r);
// 	
// 	QString s = v.toString();
	
	return 0;
}
