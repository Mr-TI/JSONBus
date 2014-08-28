/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
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

#include <nodebus/core/sharedptr.h>
#include <nodebus/core/logger.h>
#include <nodebus/core/selector.h>
#include <nodebus/core/serversocketchannel.h>
#include <nodebus/core/selectionkey.h>
#include <nodebus/core/socketchannel.h>
#include <nodebus/core/parser.h>
#include <nodebus/core/serializer.h>
#include <nodebus/core/filechannel.h>
#include <nodebus/core/idlparser/driver.h>
#include <unistd.h>
#include <fcntl.h>

using namespace NodeBus;
using namespace std;

class A: public SharedData {};

class B: public A {
public:
	inline B(int i): i(i) {}
	inline ~B() {logFiner() << "destroy B";}
	int i;
};

class C: public A {
public:
	inline C(int i): i(i) {}
	inline ~C() {logFiner() << "destroy C";}
	int i;
};

SharedPtr<B> newB() {
	return new B(2);
}

void dump(SharedPtr<B> b) {
	logInfo() << "b: " << b->i;
}

void testPtr() {
	SharedPtr<C> c = nullptr;
	SharedPtr<B> b = newB();
	SharedPtr<A> ab = b, ac = new C(1);
	SharedPtr<A> f, g, h;
	f = g = h = b;
	
// 	c = ab; // throw InvalidClassException
	c = ac;
	
	dump(ab);
	logFine() << h;
	ab = b = nullptr;
	logInfo() << "ab = b = nullptr";
	logFine() << h;
	logInfo() << "f = null";
	f = nullptr;
	logFine() << h;
	logInfo() << "g = null";
	g = 0;
	logFine() << h;
	logInfo() << "h = null";
	h = nullptr;
	logFine() << h;
	
	logInfo() << "(  ab == nullptr) is " << (ab == nullptr);
	
	logFine() << "ac => " << ac;
	logFine() << " c => " << c;
	logInfo() << "(ac == c) is " << (ac == c);
	
	
	logFine() << b;
	
// 	b->i = 0; // throw NullPointerException
	
	logInfo() << "DONE!";
}

void testSelect() {
	Selector selector;
	SharedPtr<ServerSocketChannel> server = new ServerSocketChannel("::1", 3333);
	
	server->registerTo(selector, SelectionKey::OP_READ);

	server->accept();
}

void testBCONParser() {
	QVariant v = Parser(new FileChannel("test.json", 0), FileFormat::JSON).parse();
// 	logFiner() << Logger::dump(v);
	Serializer(new FileChannel("test.bcon", O_CREAT | O_TRUNC | O_WRONLY), FileFormat::BCON).serialize(v);
	v = Parser(new FileChannel("test.bcon", 0), FileFormat::BCON).parse();
// 	logFiner() << Logger::dump(v);
	Serializer(new FileChannel("test_BCON.json", O_CREAT | O_TRUNC | O_WRONLY), FileFormat::JSON).serialize(v);
}

void testBSONParser() {
	QVariant v = Parser(new FileChannel("test.json", 0), FileFormat::JSON).parse();
// 	logFiner() << Logger::dump(v);
	Serializer(new FileChannel("test.bson", O_CREAT | O_TRUNC | O_WRONLY), FileFormat::BSON).serialize(v);
	v = Parser(new FileChannel("test.bson", 0), FileFormat::BSON).parse();
// 	logFiner() << Logger::dump(v);
	Serializer(new FileChannel("test_BSON.json", O_CREAT | O_TRUNC | O_WRONLY), FileFormat::JSON).serialize(v);
}

void testIDLCompile(const char *filename) {
	QString name(filename);
// 	try {
		QVariant v = idlparser::Driver::parse(name);
		Serializer(new FileChannel(name + ".json", O_CREAT | O_TRUNC | O_WRONLY), FileFormat::JSON).serialize(v);
		Serializer(new FileChannel(name + ".bcon", O_CREAT | O_TRUNC | O_WRONLY), FileFormat::BCON).serialize(v);
		Serializer(new FileChannel(name + ".bson", O_CREAT | O_TRUNC | O_WRONLY), FileFormat::BSON).serialize(v);
		logInfo() << Serializer::toJSONString(v, Serializer::INDENT(2));
// 	} catch (Exception &e) {
// 		logCrit() << "Compilation failed:\n" << e.message();
// 	}
}

int main(int argc, char **argv) {
	try {
		if (argc != 2) {
			throw Exception("Missing filename");
		}
	} catch (Exception &e) {
		logCrit() << "terminate called after throwing an instance of " << e;
	}
		testIDLCompile(argv[1]);
	return 0;
}
