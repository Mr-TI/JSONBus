
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/jsonparser/driver.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>

using namespace JSONBus;
using namespace std;



int main(int argc, char **argv) {

	jsonparser::Driver driver;
	
	if (!driver.parse(&cin))
		return 1;
	
	QVariant &v = driver.getResult();
	
	return 0;
}
