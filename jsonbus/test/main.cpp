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

#include <jsonbus/core/sharedptr.h>
#include <jsonbus/core/logger.h>

using namespace JSONBus;
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

void test1(SharedPtr<B> b) {
	logInfo() << "b: " << b->i;
}

void test2(SharedPtr<C> c) {
	logInfo() << "c: " << c->i;
}

int main(int argc, char **argv) {
	
	try {
		SharedPtr<A> a1 = new B(1);
		SharedPtr<C> c = new C(1);
		SharedPtr<A> a2 = c;
		
		test1(a1);
		a1 = a2;
		
		logInfo() << "a1 == nullptr : " << (a1 == nullptr? "True":"False");
		logInfo() << "null == nullptr : " << (null == nullptr? "True":"False");
		
		logFine() << "a2 : " << a2;
		logFine() << "c : " << c;
		logInfo() << "a2 == c : " << (a2 == c? "True":"False");
		
		SharedPtr<B> b;
		
		logFine() << null;
		logFine() << b;
		
		test1(a2);
		b->i = 0;
		
	} catch (Exception &e) {
		logCrit() << "terminate called after throwing an " << e;
	}
	
	return 0;
}
