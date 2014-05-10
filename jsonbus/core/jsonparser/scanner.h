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

#include <parser.hh>

/**
 * @namespace
 */
namespace jsonparser {

/**
 * @brief JSON scanner
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class Scanner : public jsonparserFlexLexer {
public:
    Scanner(std::istream* arg_yyin = 0, std::ostream* arg_yyout = 0);

    virtual ~Scanner();

    virtual Parser::token_type yylex(Parser::semantic_type *yylval);

    void set_debug(bool b);
};

}

#endif // SCANNER_H
