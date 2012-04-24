
#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/jsonparser/driver.h>
#include <jsonbus/core/jsonparser/exception.h>
#include <jsonbus/core/jsonparser/iodevicebuf.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>
#include <QFile>
#include <fstream>
#include <qjson/parser.h>

using namespace JSONBus;
using namespace jsonparser;
using namespace std;

int main(int argc, char **argv) {
	Driver driver;
	QVariant v = driver.parse(&cin);
	qDebug() << v;
	v = driver.parse();
	qDebug() << v;
	
// 	ifstream in;
// 	in.open("/home/versche1/src/cpp/jsonbus/jsontest", ios::in);
	
// 	QFile file("jsontest");
// 	file.open(QIODevice::ReadOnly);
	
// 	IODeviceBuf buf(file);
// 	istream in(&buf);
// 	QVariant v = driver.parse(&in);
	
// 	for (int i = 0; i < 10000; i++) {
// 		QFile file("jsontest");
// 		file.open(QIODevice::ReadOnly);
// 		bool ok;
// 		QJson::Parser parser;
// 		QVariant v = parser.parse (&file, &ok);
// 		IODeviceBuf buf(file);
// 		istream in(&buf);
// 		Driver driver;
// 		ifstream in;
// 		in.open("jsontest", ios::in);
// 		QVariant v = driver.parse(&in);
// 	}
	
// 	qDebug() << v;
	
// 	cout << "servlet-name: " << v.toMap()["web-app"].toMap()["servlet"].toList()[0].toMap()["servlet-name"].toString() << endl;
	
	return 0;
}
