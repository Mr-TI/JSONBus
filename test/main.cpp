
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>

using namespace jsonbus;
using namespace std;



int main(int argc, char **argv) {
	try {
		QCoreApplication app(argc, argv);
		
	} catch (Exception e) {
		cout << "Exception: " << e.getMessage() << endl;
	} catch (...) {
		cout << "Unknow exception." << endl;
	}
	return 0;
}
