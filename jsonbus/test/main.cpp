/*
 *   Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *		   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <jsonbus/core/common.h>
#include <jsonbus/core/exception.h>
#include <jsonbus/core/jsonparser/driver.h>
#include <jsonbus/core/jsonparser/exception.h>
#include <jsonbus/core/jsonparser/iodevicebuf.h>

#include <QStringList>
#include <QCoreApplication>
#include <QMap>
#include <QFile>
#include <iostream>
#include <jsonbus/core/jsonparser/stdstreambuf.h>
#include <jsonbus/core/sharedptr.h>
#include <jsonbus/shareddata.h>

using namespace JSONBus;
using namespace jsonparser;
using namespace std;

class A: public SharedData {};

class B: public A {
public:
	inline B(int i): i(i) {}
	inline ~B() {printf("destroy\n");}
	int i;
};

void test(SharedPtr<B> b) {
	printf("b=%d\n", b->i);
}

int main(int argc, char **argv) {
	
	SharedPtr<A> a = new B(1);
	test(a);
	
	SharedPtr<B> b;
	
	printf("test=%s\n", (b == null? "True":"False"));
	
	b->i = 5;
	test(b);
	
// 	fstream fifoStream;
// 	fifoStream.open("/tmp/jsonbus", fstream::in);
// 	StdStreamBuf buf(fifoStream);
// 	istream inputStream(&buf);
// 	char c = EOF;
// 	
// 	for (int i=0; i<10; i++) {
// 		inputStream >> c;
// 		if (c == EOF)
// 			break;
// 		cout << "c:" << c << "(" << (int)c << ")" << endl;
// 	}
	
// 	Driver driver;
// 	QVariant v = driver.parse(&cin);
// 	qDebug() << v;
// 	v = driver.parse();
// 	qDebug() << v;
	
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
