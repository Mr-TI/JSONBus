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
