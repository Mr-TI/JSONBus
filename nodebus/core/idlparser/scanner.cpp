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

#include <ltype.h>
#include <scanner.h>
#include <logger.h>
#include <stdio.h>

namespace idlparser {

Scanner::Scanner(const QString& filename)
: idlparserFlexLexer(nullptr, nullptr), m_stream(fopen(filename.toLocal8Bit().constData(), "r")) {
	if (!m_stream) {
		throw IOException("Fail to open " + filename + ", " + QString::fromLocal8Bit(strerror(errno)));
	}
}

Scanner::~Scanner() {
	fclose(m_stream);
}

int Scanner::LexerInput( char* buf, int max_size ) {
	return fread(buf, 1,max_size,  m_stream);
// 	size_t n = fread(buf, 1, 1,  m_stream);
// 	std::cout << "C=" << buf[0] << std::endl;
// 	return n;
}

}

#ifdef yylex
#undef yylex
#endif

int idlparserFlexLexer::yylex() {
    std::cerr << "call idlparserFlexLexer::yylex()!" << std::endl;
    return 0;
}
