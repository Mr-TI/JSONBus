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

#include <globals.h>
#include <scanner.h>
#include <logger.h>

namespace jsonparser {

Scanner::Scanner(NodeBus::DataStream &dataStream)
        : jsonparserFlexLexer(nullptr, nullptr), m_dataStream(dataStream) {
}

Scanner::~Scanner() {
}

int Scanner::LexerInput( char* buf, int max_size ) {
	return m_dataStream.read(buf, 1);
}

}

#ifdef yylex
#undef yylex
#endif

int jsonparserFlexLexer::yylex() {
    std::cerr << "call jsonparserFlexLexer::yylex()!" << std::endl;
    return 0;
}
