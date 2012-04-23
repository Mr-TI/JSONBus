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

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur <emericv@openihs.org> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur <emericv@openihs.org> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

%{
#include <iostream>
using namespace std;

#include <globals.h>
#include <parser.hh>
#include <string>
#include <scanner.h>
#define CAPTURE_TEXT() yylval->field = new string(yytext)
// #define RETURN_TOKEN(label) cout << #label << "|"; return token::label
#define RETURN_TOKEN(label) return token::label
typedef jsonparser::Parser::token token;
typedef jsonparser::Parser::token_type token_type;
#define yyterminate() RETURN_TOKEN(TEND)

%}

space					[\t\n\r ]

kw_null					null
kw_true					true
kw_false				false
kw_nan					nan
kw_infinity				infinity

number					-?[[:digit:]]+(\.[[:digit:]]+)?
num_exp					e

coma					,
semicolon				:
dblcote					\"
square_bracket_open		\{
square_bracket_close	\}
curly_bracket_open		\[
curly_bracket_close		\]

value_unicode			[[:digit:]]{4}

escape_unicode			u
escape_backslash		\\
escape_dblcote			\"
escape_slash			\/
escape_backspace		b
escape_formfeed			f
escape_newline			n
escape_car_ret			r
escape_horiz_tab		t
escape_prefix			\\

%s __STATE__

%option noyywrap yylineno c++
%option prefix="jsonparser"
%option batch
%option align

%%

<<EOF>>							{BEGIN INITIAL; RETURN_TOKEN(TEND);}

<INITIAL>{kw_null}				{BEGIN __STATE__; RETURN_TOKEN(TNULL);}
<INITIAL>{kw_true}				{BEGIN __STATE__; RETURN_TOKEN(TTRUE);}
<INITIAL>{kw_false}				{BEGIN __STATE__; RETURN_TOKEN(TFALSE);}
<INITIAL>{kw_nan}				{BEGIN __STATE__; RETURN_TOKEN(TNAN);}
<INITIAL>{kw_infinity}			{BEGIN __STATE__; RETURN_TOKEN(TINFINITY);}
<INITIAL>{number}/e				{BEGIN NUMBER; RETURN_TOKEN(TNUMBER);}
<INITIAL>{number}				{BEGIN NUMBER;}
<INITIAL>{square_bracket_open}	{BEGIN __STATE__; RETURN_TOKEN(TTBLBEGIN);}
<INITIAL>{square_bracket_close}	{BEGIN __STATE__; RETURN_TOKEN(TTBLEND);}
<INITIAL>{curly_bracket_open}	{BEGIN __STATE__; RETURN_TOKEN(TOBJBEGIN);}
<INITIAL>{curly_bracket_close}	{BEGIN __STATE__; RETURN_TOKEN(TOBJEND);}
<INITIAL>{coma}					{BEGIN __STATE__; RETURN_TOKEN();}
<INITIAL>{semicolon}			{BEGIN __STATE__; RETURN_TOKEN();}
<INITIAL>{dblcote}				{BEGIN __STATE__; RETURN_TOKEN();}
<INITIAL>{value_unicode}		{BEGIN __STATE__; RETURN_TOKEN();}

%%
