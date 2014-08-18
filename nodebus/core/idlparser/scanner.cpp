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

#include <ltype.h>
#include <scanner.h>
#include <logger.h>
#include <stdio.h>

#define THROW_IOEXP_ON_NULL(exp) \
	if ((exp) == NULL) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(strerror(errno)))

#define THROW_IOEXP_ON_ERR(exp) \
	if ((exp) == -1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + ": " + QString::fromLocal8Bit(strerror(errno)))

namespace idlparser {

Scanner::Scanner(const QString& filename)
        : idlparserFlexLexer(nullptr, nullptr), m_stream(fopen(filename.toLocal8Bit().constData(), "r")) {
	THROW_IOEXP_ON_NULL(m_stream);
}

Scanner::~Scanner() {
	fclose(m_stream);
}

int Scanner::LexerInput( char* buf, int max_size ) {
	return fread(buf, 1,max_size,  m_stream);
}

}

#ifdef yylex
#undef yylex
#endif

int idlparserFlexLexer::yylex() {
    std::cerr << "call idlparserFlexLexer::yylex()!" << std::endl;
    return 0;
}
