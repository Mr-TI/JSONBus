/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@openihs.org>
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
#include <nodebus/core/jsonparser.h>
#include <nodebus/core/bconserializer.h>
#include <nodebus/core/filechannel.h>
#include <nodebus/core/jsonserializer.h>
#include <nodebus/core/bconparser.h>
#include <nodebus/core/bsonparser.h>
#include <nodebus/core/bsonserializer.h>
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
	StreamChannelPtr file = new FileChannel("test.json", 0);
	QVariant v = JSONParser(file).parse();
	logFiner() << Logger::dump(v);
	BCONSerializer(new FileChannel("test.cson", O_CREAT | O_TRUNC | O_WRONLY)).serialize(v);
	file = new FileChannel("test.cson", 0);
	v = BCONParser(file).parse();
	logFiner() << Logger::dump(v);
	JSONSerializer(new FileChannel("test0.json", O_CREAT | O_TRUNC | O_WRONLY)).serialize(v);
}

void testBSONParser() {
	StreamChannelPtr file = new FileChannel("test.json", 0);
	QVariant v = JSONParser(file).parse();
	logFiner() << Logger::dump(v);
	BSONSerializer(new FileChannel("test.bson", O_CREAT | O_TRUNC | O_WRONLY)).serialize(v);
	file = new FileChannel("test.bson", 0);
	v = BSONParser(file).parse();
	logFiner() << Logger::dump(v);
	JSONSerializer(new FileChannel("test0.json", O_CREAT | O_TRUNC | O_WRONLY)).serialize(v);
}

int main(int argc, char **argv) {
// 	try {
		testBSONParser();
// 	} catch (Exception &e) {
// 		logCrit() << "terminate called after throwing an instance of " << e;
// 	}
	return 0;
}
