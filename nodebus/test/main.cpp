/*
 * Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <nodebus/core/sharedptr.h>
#include <nodebus/core/logger.h>
#include <nodebus/nio/selector.h>
#include <nodebus/nio/serversocketchannel.h>
#include <nodebus/nio/selectionkey.h>
#include <nodebus/nio/socketchannel.h>
#include <nodebus/core/parser.h>
#include <nodebus/core/serializer.h>
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

// void testSelect() {
// 	Selector selector;
// 	SharedPtr<ServerSocketChannel> server = new ServerSocketChannel("::1", 3333);
// 	
// 	server->registerTo(selector, SelectionKey::OP_READ);
// 
// 	server->accept();
// }

void testBCONParser() {
	QVariant v = Parser::fromFile("test/test.json", FileFormat::JSON);
// 	logFiner() << Logger::dump(v);
	Serializer::toFile("test/test.bcon", v, FileFormat::BCON);
	v = Parser::fromFile("test/test.bcon", FileFormat::BCON);
// 	logFiner() << Logger::dump(v);
	Serializer::toFile("test/test_BCON.json", v, FileFormat::JSON, Serializer::INDENT(2));
}

void testBSONParser() {
	QVariant v = Parser::fromFile("test/test.json", FileFormat::JSON);
// 	logFiner() << Logger::dump(v);
	Serializer::toFile("test/test.bson", v, FileFormat::BSON);
	v = Parser::fromFile("test/test.bson", FileFormat::BSON);
// 	logFiner() << Logger::dump(v);
	Serializer::toFile("test/test_BSON.json", v, FileFormat::JSON, Serializer::INDENT(2));
}

void testIDLCompile(const char *filename) {
	QString name(filename);
	try {
		QVariant v = idlparser::Driver::parse(name);
// 		Serializer::toFile(name + ".json", v, FileFormat::JSON);
// 		Serializer::toFile(name + ".bcon", v, FileFormat::BCON);
// 		Serializer::toFile(name + ".bson", v, FileFormat::BSON);
		logInfo() << Serializer::toJSONString(v, Serializer::INDENT(2));
	} catch (Exception &e) {
		logCrit() << "Compilation failed:\n" << e.message();
	}
}

int main(int argc, char **argv) {
	try {
// 		if (argc != 2) {
// 			throw Exception("Missing filename");
// 		}
// 		testIDLCompile(argv[1]);
		testBCONParser();
// 		testBSONParser();
	} catch (Exception &e) {
		logCrit() << "terminate called after throwing an instance of " << e;
	}
	return 0;
}
