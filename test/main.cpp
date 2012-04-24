
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/jsonparser/driver.h>
#include <jsonbus/core/jsonparser/exception.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>
#include <fstream>

using namespace JSONBus;
using namespace std;



int main(int argc, char **argv) {

	jsonparser::Driver driver;
	
	ifstream in;
	in.open("/home/versche1/src/cpp/jsonbus/jsontest", ios::in);
	
	QVariant &v = driver.parse(&in);
	
	cout << "FirstName: " << v.toMap()["FirstName"].toString() << endl;
	cout << "Amount: " << v.toMap()["amount"].toDouble() << endl;
	
	return 0;
}
