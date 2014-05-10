/*
 *   Copyright 2014 Emeric Verschuur <emericv@openihs.org>
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

%{
#include <iostream>
using namespace std;

#define YY_READ_BUF_SIZE 1

#include <globals.h>
#include <parser.hh>
#include <string>
#include <scanner.h>

#define VARIANT_TXT2(_type_)\
	yylval->node = new variant_t(yytext);\
	yylval->node->convert(variant_t::_type_)

#define VARIANT_FROM(_value_)\
	yylval->node = new variant_t(_value_)

#define VARIANT_INVALID()\
	yylval->node = new variant_t()
	
#define VARIANT_INFINITY()\
	yylval->node = new variant_t(variant_t::Double);\
	yylval->node->setValue(yytext[0] == '-'?\
	-std::numeric_limits<double>::infinity():\
	std::numeric_limits<double>::infinity())
	
#define VARIANT_NAN()\
	yylval->node = new variant_t(variant_t::Double);\
	yylval->node->setValue(std::numeric_limits<double>::quiet_NaN())

#define STRING_BEGIN()\
	yylval->str = new string_t()

#define STRING_APPEND(_val_)\
	yylval->str->append(_val_);

#define STRING_APPEND_UNICODE()\
	yylval->str->append(QChar(string_t(yytext).toInt(0, 16)));

#define STRING_CLEAR()\
	delete yylval->str;

#undef yywrap
#define yywrap() 1

#define RETURN_TOKEN(_label_) /*cout << #_label_ << endl;*/ return token::_label_
typedef jsonparser::Parser::token token;
typedef jsonparser::Parser::token_type token_type;
#define yyterminate() RETURN_TOKEN(TEND)

%}

space                   [\t\n\r ]

kw_null                 null
kw_undefined            undefined
kw_true                 true
kw_false                false
kw_nan                  nan
kw_infinity             [\-\+]?infinity

number                  \-?[[:digit:]]+(\.[[:digit:]]+)?(e[\-\+]?[[:digit:]]+)?

coma                    ,
semicolon               :
dblcote                 \"
square_bracket_open     \[
square_bracket_close    \]
curly_bracket_open      \{
curly_bracket_close     \}

value_unicode           [0-9a-f]{4}

escape_unicode          u
escape_backslash        \\
escape_dblcote          \"
escape_slash            \/
escape_backspace        b
escape_formfeed         f
escape_newline          n
escape_car_ret          r
escape_horiz_tab        t
escape_prefix           \\

%s STRING STRING_ESCAPE STRING_ESCAPE_UNICODE

%option noyywrap yylineno c++
/* %option batch never-interactive */
%option prefix="jsonparser"
%option align
%option case-insensitive

%option nounput debug

%%

<<EOF>>                                   {BEGIN INITIAL; RETURN_TOKEN(TEND);}

<STRING>{dblcote}                         {BEGIN INITIAL; RETURN_TOKEN(TSTRING);}
<STRING>{escape_prefix}                   {BEGIN STRING_ESCAPE;}
<STRING>.                                 {BEGIN STRING; STRING_APPEND(yytext[0]);}

<STRING_ESCAPE>{escape_unicode}           {BEGIN STRING_ESCAPE_UNICODE; }
<STRING_ESCAPE>{escape_backslash}         {BEGIN STRING; STRING_APPEND('\\');}
<STRING_ESCAPE>{escape_dblcote}           {BEGIN STRING; STRING_APPEND('\"');}
<STRING_ESCAPE>{escape_slash}             {BEGIN STRING; STRING_APPEND('/');}
<STRING_ESCAPE>{escape_backspace}         {BEGIN STRING; STRING_APPEND('\b');}
<STRING_ESCAPE>{escape_formfeed}          {BEGIN STRING; STRING_APPEND('\f');}
<STRING_ESCAPE>{escape_newline}           {BEGIN STRING; STRING_APPEND('\n');}
<STRING_ESCAPE>{escape_car_ret}           {BEGIN STRING; STRING_APPEND('\r');}
<STRING_ESCAPE>{escape_horiz_tab}         {BEGIN STRING; STRING_APPEND('\t');}
<STRING_ESCAPE>.                          {BEGIN INITIAL; STRING_CLEAR(); RETURN_TOKEN(TSYNERRESC);}

<STRING_ESCAPE_UNICODE>{value_unicode}    {BEGIN STRING; STRING_APPEND_UNICODE();}
<STRING_ESCAPE_UNICODE>.                  {BEGIN INITIAL; STRING_CLEAR(); RETURN_TOKEN(TSYNERRUNI);}

<INITIAL>{kw_null}                        {BEGIN INITIAL; VARIANT_INVALID(); RETURN_TOKEN(TVARIANT);}
<INITIAL>{kw_undefined}                   {BEGIN INITIAL; VARIANT_INVALID(); RETURN_TOKEN(TVARIANT);}
<INITIAL>{kw_true}                        {BEGIN INITIAL; VARIANT_FROM(true); RETURN_TOKEN(TVARIANT);}
<INITIAL>{kw_false}                       {BEGIN INITIAL; VARIANT_FROM(false); RETURN_TOKEN(TVARIANT);}
<INITIAL>{kw_nan}                         {BEGIN INITIAL; VARIANT_NAN(); RETURN_TOKEN(TVARIANT);}
<INITIAL>{kw_infinity}                    {BEGIN INITIAL; VARIANT_INFINITY(); RETURN_TOKEN(TVARIANT);}

<INITIAL>{number}                         {BEGIN INITIAL; VARIANT_TXT2(Double); RETURN_TOKEN(TVARIANT);}

<INITIAL>{square_bracket_open}            {BEGIN INITIAL; RETURN_TOKEN(TARRBEGIN);}
<INITIAL>{square_bracket_close}           {BEGIN INITIAL; RETURN_TOKEN(TARREND);}
<INITIAL>{curly_bracket_open}             {BEGIN INITIAL; RETURN_TOKEN(TOBJBEGIN);}
<INITIAL>{curly_bracket_close}            {BEGIN INITIAL; RETURN_TOKEN(TOBJEND);}
<INITIAL>{coma}                           {BEGIN INITIAL; RETURN_TOKEN(TELEMENTSEP);}
<INITIAL>{semicolon}                      {BEGIN INITIAL; RETURN_TOKEN(TMEMBERSEP);}
<INITIAL>{dblcote}                        {BEGIN STRING; STRING_BEGIN();}
<INITIAL>{space}                          {}
<INITIAL>.                                {RETURN_TOKEN(TSYNERR);}

%%
