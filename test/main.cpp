
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/settings.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>

using namespace jsonbus;
using namespace std;



int main(int argc, char **argv) {
	try {
		QCoreApplication app(argc, argv);
		Settings settings("Openihs.org", "Test");
		settings.define("uri", "Set the service URI", "https://openihs.org/ws");
		settings.define("login", "Login name to connect to the service", "admin");
		settings.define("passwd", "Password to connect to the service", "passwd");
		settings.setup();
	} catch (Exception e) {
		cout << "Exception: " << e.getMessage() << endl;
	} catch (...) {
		cout << "Unknow exception." << endl;
	}
	return 0;
}
