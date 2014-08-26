/*
 *   Copyright 2014 Emeric Verschuur <emericv@mbedsys.org>
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

%top{

#define YY_TYPEDEF_YY_SIZE_T
typedef int yy_size_t;

}

%{
#include <iostream>
using namespace std;

#define YY_READ_BUF_SIZE 1

#include "ltype.h"
#include "nodevariant.h"
#include <parser.hh>
#include <string>
#include <scanner.h>

#define VARIANT_STRING()\
	yylval->node  = new NodeVariant(yytext);

#define VARIANT_TXT2(_type_)\
	yylval->node  = new NodeVariant(yytext);\
	yylval->node->val().convert(QVariant::_type_)

#define VARIANT_HEXNUM()\
	yylval->node  = new NodeVariant(QString(yytext + 2).toInt(0, 16));\
	yylval->node->val().convert(QVariant::LongLong);\
	if ((yylval->node->val().toLongLong() & 0xFFFFFFFF00000000) == 0)\
	yylval->node->val().convert(QVariant::Int)

#define VARIANT_OCTNUM()\
	yylval->node  = new NodeVariant(QString(yytext + 1).toInt(0, 8));\
	yylval->node->val().convert(QVariant::LongLong);\
	if ((yylval->node->val().toLongLong() & 0xFFFFFFFF00000000) == 0)\
	yylval->node->val().convert(QVariant::Int)

#define VARIANT_BINNUM()\
	yylval->node  = new NodeVariant(QString(yytext + 2).toInt(0, 2));\
	yylval->node->val().convert(QVariant::LongLong);\
	if ((yylval->node->val().toLongLong() & 0xFFFFFFFF00000000) == 0)\
	yylval->node->val().convert(QVariant::Int)

#define VARIANT_NUMBER()\
	yylval->node  = new NodeVariant(yytext);\
	yylval->node->val().convert(QVariant::LongLong);\
	if ((yylval->node->val().toLongLong() & 0xFFFFFFFF00000000) == 0)\
	yylval->node->val().convert(QVariant::Int)

#define VARIANT_NULL()\
	yylval->node  = new NodeVariant(QVariant())

#define VARIANT_FROM(_value_)\
	yylval->node  = new NodeVariant(_value_)

#define STRING_BEGIN()\
	yylval->str = new QString()

#define STRING_APPEND(_val_)\
	yylval->str->append(_val_);

#define STRING_APPEND_UNICODE()\
	yylval->str->append(QChar(QString(yytext).toInt(0, 16)));

#define STRING_END()\
	yylval->node  = new NodeVariant(*(yylval->str))

#define STRING_CLEAR()\
	delete yylval->str;yylval->str = NULL;

#undef yywrap
#define yywrap() 1

#define RETURN_TOKEN(_label_) /*cout << #_label_ << endl;*/ return token::_label_
typedef idlparser::Parser::token token;
typedef idlparser::Parser::token_type token_type;
#define yyterminate() RETURN_TOKEN(TEND)

%}

space                   [\t\n\r ]
newline                 \n

kw_include              ^[\t\n\r ]*#[\t\n\r ]*include[\t\n\r ]*<[\t\n\r ]*

kw_null                 null
kw_true                 true
kw_false                false

kw_module               module
kw_interface            interface

kw_void                 void
kw_boolean              boolean
kw_char                 char
kw_wchar                wchar
kw_float                float
kw_double               double
kw_long                 long
kw_short                short
kw_octet                octet
kw_string               string
kw_object               Object
kw_datetime             DateTime
kw_any                  any

kw_unsigned             unsigned
kw_const                const
kw_readonly             readonly
kw_attribute            attribute

kw_enum                 enum
kw_struct               struct
kw_union                union
kw_switch               switch
kw_case                 case
kw_default              default
kw_oneway               oneway
kw_in                   in
kw_out                  out
kw_inout                inout
kw_sequence             sequence
kw_exception            exception
kw_raises               raises

kw_typedef              typedef

integer                 [[:digit:]]+
number                  \-?[[:digit:]]+(e[\-\+]?[[:digit:]]+)?
float                   \-?[[:digit:]]+(\.[[:digit:]]+)?(e[\-\+]?[[:digit:]]+)?
hexnum                  \-?0[xX][0-9a-fA-F]+
octnum                  \-?0[0-7]+
binnum                  \-?0[bB][01]+

symbol                  [a-zA-Z_][a-zA-Z0-9_]*

dblcote                 \"
less_than               <
greater_than            >

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
comment_open            "/*"
comment_end             "*/"
comment_line            "//".*$

%s STRING STRING_ESCAPE STRING_ESCAPE_UNICODE INCLUDE COMMENT

%option noyywrap yylineno c++
/*%option batch never-interactive*/
%option prefix="idlparser"
%option align
%option case-sensitive

%option nounput debug

%%

<<EOF>>                                   {BEGIN INITIAL; RETURN_TOKEN(TEND);}

<STRING>{dblcote}                         {BEGIN INITIAL; STRING_END(); RETURN_TOKEN(TSTRINGVAL);}
<STRING>{escape_prefix}                   {BEGIN STRING_ESCAPE;}
<STRING>.                                 {BEGIN STRING; STRING_APPEND(yytext[0]);}

<INCLUDE>[^\n]*{greater_than}             {BEGIN INITIAL; yytext[yyleng - 1] = '\0';VARIANT_STRING(); RETURN_TOKEN(TINCLUDE);}
<INCLUDE>{newline}                        {BEGIN INITIAL; RETURN_TOKEN(TSYNERR);}
<INCLUDE>.                                {BEGIN INCLUDE;}

<COMMENT>{comment_end}                    {BEGIN INITIAL;}
<COMMENT>{space}                          {BEGIN COMMENT;}
<COMMENT>.                                {BEGIN COMMENT;}

<STRING_ESCAPE>{escape_unicode}           {BEGIN STRING_ESCAPE_UNICODE; }
<STRING_ESCAPE>{escape_backslash}         {BEGIN STRING; STRING_APPEND('\\');}
<STRING_ESCAPE>{escape_dblcote}           {BEGIN STRING; STRING_APPEND('\"');}
<STRING_ESCAPE>{escape_slash}             {BEGIN STRING; STRING_APPEND('/');}
<STRING_ESCAPE>{escape_backspace}         {BEGIN STRING; STRING_APPEND('\b');}
<STRING_ESCAPE>{escape_formfeed}          {BEGIN STRING; STRING_APPEND('\f');}
<STRING_ESCAPE>{escape_newline}           {BEGIN STRING; STRING_APPEND('\n');}
<STRING_ESCAPE>{escape_car_ret}           {BEGIN STRING; STRING_APPEND('\r');}
<STRING_ESCAPE>{escape_horiz_tab}         {BEGIN STRING; STRING_APPEND('\t');}
<STRING_ESCAPE>.                          {BEGIN INITIAL; RETURN_TOKEN(TSYNERRESC);}

<STRING_ESCAPE_UNICODE>{value_unicode}    {BEGIN STRING; STRING_APPEND_UNICODE();}
<STRING_ESCAPE_UNICODE>.                  {BEGIN INITIAL; RETURN_TOKEN(TSYNERRUNI);}

<INITIAL>{kw_include}                     {BEGIN INCLUDE; }

<INITIAL>{kw_null}                        {BEGIN INITIAL; VARIANT_NULL(); RETURN_TOKEN(TVARIANT);}
<INITIAL>{kw_true}                        {BEGIN INITIAL; VARIANT_FROM(true); RETURN_TOKEN(TVARIANT);}
<INITIAL>{kw_false}                       {BEGIN INITIAL; VARIANT_FROM(false); RETURN_TOKEN(TVARIANT);}

<INITIAL>{hexnum}                         {BEGIN INITIAL; VARIANT_HEXNUM(); RETURN_TOKEN(TINTVAL);}
<INITIAL>{octnum}                         {BEGIN INITIAL; VARIANT_OCTNUM(); RETURN_TOKEN(TINTVAL);}
<INITIAL>{binnum}                         {BEGIN INITIAL; VARIANT_BINNUM(); RETURN_TOKEN(TINTVAL);}
<INITIAL>{integer}                        {BEGIN INITIAL; VARIANT_NUMBER(); RETURN_TOKEN(TNUMBER);}
<INITIAL>{number}                         {BEGIN INITIAL; VARIANT_NUMBER(); RETURN_TOKEN(TINTVAL);}
<INITIAL>{float}                          {BEGIN INITIAL; VARIANT_TXT2(Double); RETURN_TOKEN(TFLOATVAL);}

<INITIAL>{kw_module}                      {BEGIN INITIAL; RETURN_TOKEN(TMODULE);}
<INITIAL>{kw_interface}                   {BEGIN INITIAL; RETURN_TOKEN(TINTERFACE);}

<INITIAL>{kw_void}                        {BEGIN INITIAL; RETURN_TOKEN(TVOID);}
<INITIAL>{kw_boolean}                     {BEGIN INITIAL; RETURN_TOKEN(TBOOLEAN);}
<INITIAL>{kw_char}                        {BEGIN INITIAL; RETURN_TOKEN(TCHAR);}
<INITIAL>{kw_wchar}                       {BEGIN INITIAL; RETURN_TOKEN(TWCHAR);}
<INITIAL>{kw_float}                       {BEGIN INITIAL; RETURN_TOKEN(TFLOAT);}
<INITIAL>{kw_double}                      {BEGIN INITIAL; RETURN_TOKEN(TDOUBLE);}
<INITIAL>{kw_long}                        {BEGIN INITIAL; RETURN_TOKEN(TLONG);}
<INITIAL>{kw_short}                       {BEGIN INITIAL; RETURN_TOKEN(TSHORT);}
<INITIAL>{kw_octet}                       {BEGIN INITIAL; RETURN_TOKEN(TOCTET);}
<INITIAL>{kw_string}                      {BEGIN INITIAL; RETURN_TOKEN(TSTRING);}
<INITIAL>{kw_object}                      {BEGIN INITIAL; RETURN_TOKEN(TOBJECT);}
<INITIAL>{kw_datetime}                    {BEGIN INITIAL; RETURN_TOKEN(TDATETIME);}
<INITIAL>{kw_any}                         {BEGIN INITIAL; RETURN_TOKEN(TANY);}

<INITIAL>{kw_unsigned}                    {BEGIN INITIAL; RETURN_TOKEN(TUNSIGNED);}
<INITIAL>{kw_const}                       {BEGIN INITIAL; RETURN_TOKEN(TCONST);}
<INITIAL>{kw_readonly}                    {BEGIN INITIAL; RETURN_TOKEN(TREADONLY);}
<INITIAL>{kw_attribute}                   {BEGIN INITIAL; RETURN_TOKEN(TATTRIBUTE);}

<INITIAL>{kw_enum}                        {BEGIN INITIAL; RETURN_TOKEN(TENUM);}
<INITIAL>{kw_struct}                      {BEGIN INITIAL; RETURN_TOKEN(TSTRUCT);}
<INITIAL>{kw_union}                       {BEGIN INITIAL; RETURN_TOKEN(TUNION);}
<INITIAL>{kw_switch}                      {BEGIN INITIAL; RETURN_TOKEN(TSWITCH);}
<INITIAL>{kw_case}                        {BEGIN INITIAL; RETURN_TOKEN(TCASE);}
<INITIAL>{kw_default}                     {BEGIN INITIAL; RETURN_TOKEN(TDEFAULT);}
<INITIAL>{kw_oneway}                      {BEGIN INITIAL; RETURN_TOKEN(TONEWAY);}
<INITIAL>{kw_in}                          {BEGIN INITIAL; RETURN_TOKEN(TIN);}
<INITIAL>{kw_out}                         {BEGIN INITIAL; RETURN_TOKEN(TOUT);}
<INITIAL>{kw_inout}                       {BEGIN INITIAL; RETURN_TOKEN(TINOUT);}
<INITIAL>{kw_sequence}                    {BEGIN INITIAL; RETURN_TOKEN(TSEQUENCE);}
<INITIAL>{kw_exception}                   {BEGIN INITIAL; RETURN_TOKEN(TEXCEPTION);}
<INITIAL>{kw_raises}                      {BEGIN INITIAL; RETURN_TOKEN(TRAISES);}

<INITIAL>{kw_typedef}                     {BEGIN INITIAL; RETURN_TOKEN(TTYPEDEF);}

<INITIAL>{dblcote}                        {BEGIN STRING; STRING_BEGIN();}

<INITIAL>{comment_open}                   {BEGIN COMMENT;}

<INITIAL>{comment_line}                   {BEGIN INITIAL;}
<INITIAL>{space}                          {BEGIN INITIAL;}

<INITIAL>{symbol}                         {BEGIN INITIAL; VARIANT_STRING(); RETURN_TOKEN(TSYMBOL);}

<INITIAL>.                                {BEGIN INITIAL; /*cout << "<<<" << yytext[0] << ">>>" << endl;*/ return yytext[0];}

%%
