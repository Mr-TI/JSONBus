/*
    Copyright (c) 2012, Emeric Verschuur <emericv@openihs.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the OpenIHS.org nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "jsonparsertask.h"
#include <typeinfo>
#include "common.h"

namespace JSONBus {

JSONParserTask::JSONParserTask(int fd, QObject* parent)
: JSONParser(fd, parent), m_stop(false) {

}

JSONParserTask::JSONParserTask(std::istream& stream, QObject* parent)
: JSONParser(stream, parent), m_stop(false) {

}

JSONParserTask::JSONParserTask(QTextStream& stream, QObject* parent)
: JSONParser(stream, parent), m_stop(false) {

}

JSONParserTask::JSONParserTask(QIODevice& input, QObject* parent)
: JSONParser(input, parent), m_stop(false) {

}

void JSONParserTask::run() {
	try {
		while (1) {
			emit dataAvailable(parse());
		}
	} catch (JSONParserException &e) {
		if (m_stop) {
			cerr << demangle(typeid(*this).name()) << " leaving main loop normally" << endl;
		} else {
			cerr << demangle(typeid(*this).name()) << " leaving main loop after throwing an instance of '" << demangle(typeid(e).name()) << "'" << endl;
			if (!e.message().isEmpty())
				cerr << "  what(): " << e.message() << endl;
		}
	}
	emit terminated();
}

void JSONParserTask::terminate() {
	m_stop = true;
	disable();
}


}

