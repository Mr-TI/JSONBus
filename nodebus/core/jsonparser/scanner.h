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

#ifndef SCANNER_H
#define SCANNER_H

#ifndef YY_DECL
#define YY_DECL jsonparser::Parser::token_type \
    jsonparser::Scanner::yylex(jsonparser::Parser::semantic_type* yylval)
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer jsonparserFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

#include <nodebus/core/datastream.h>
#include <parser.hh>

/**
 * @namespace
 */
namespace jsonparser {

/**
 * @brief JSON scanner
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */
class Scanner : public jsonparserFlexLexer {
public:
	Scanner(NodeBus::DataStream &dataStream);

	virtual ~Scanner();

	virtual Parser::token_type yylex(Parser::semantic_type *yylval);
	
	virtual void resetPos();

	virtual void set_debug(bool b);

protected:
	virtual int LexerInput( char* buf, int max_size );
	
private:
	NodeBus::DataStream &m_dataStream;
};

inline void Scanner::set_debug(bool b) {
	yy_flex_debug = b;
}

inline void Scanner::resetPos() {
	yylineno = 1;
	yyleng = 0;
}

}

#endif // SCANNER_H
