/*
    Copyright (c) 2011, Emeric Verschuur <emericv@gmail.com>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur <emericv@gmail.com> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur <emericv@gmail.com> BE LIABLE FOR ANY
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
#include <scanner.h>
#define CAPTURE_TEXT() yylval->str = new QString(yytext)
typedef argsparser::Parser::token token;
typedef argsparser::Parser::token_type token_type;
#define yyterminate() return token::TEND

%}

equal           =
argname         --[[:alpha:]_][[:alnum:]_-]*
short_arg       [[:alpha:]]
short_arg_pref  -
svalue_delimit  \"
cvalue_delimit  \'
escape          \\
dvalue          [[:digit:]]+

%s SVALUE_IN
%s SVALUE_IN_WARP
%s CVALUE_IN
%s CVALUE_IN_WARP
%s SHORT_ARG_IN

%option noyywrap never-interactive yylineno c++
%option prefix="argsparser"


%%

<<EOF>>                         {BEGIN INITIAL; return token::TEND;}                    /* End Of File */

<SVALUE_IN_WARP>.               {yymore(); BEGIN SVALUE_IN;}

<SVALUE_IN>{svalue_delimit}     {BEGIN INITIAL; CAPTURE_TEXT(); yylval->str->remove(-1); return token::TVAL;}   /* "an string value" */
<SVALUE_IN>{escape}             {yymore(); BEGIN SVALUE_IN_WARP;}
<SVALUE_IN>.                    {yymore(); BEGIN SVALUE_IN;}

<CVALUE_IN_WARP>.               {yymore(); BEGIN CVALUE_IN;}

<CVALUE_IN>{cvalue_delimit}     {BEGIN INITIAL; CAPTURE_TEXT(); yylval->str->remove(-1); return token::TVAL;}   /* 'an string value' */
<CVALUE_IN>{escape}             {yymore(); BEGIN CVALUE_IN_WARP;}
<CVALUE_IN>.                    {yymore(); BEGIN CVALUE_IN;}

<SHORT_ARG_IN>{short_arg}       {BEGIN INITIAL; CAPTURE_TEXT(); return token::TVAL;}   /* a charater */
<SHORT_ARG_IN>{dvalue}          {BEGIN INITIAL; CAPTURE_TEXT(); return token::TVAL;}
<SHORT_ARG_IN>[[:blank:]]*      {BEGIN INITIAL;}

<INITIAL>{svalue_delimit}       {BEGIN SVALUE_IN;}
<INITIAL>{cvalue_delimit}       {BEGIN CVALUE_IN;}
<INITIAL>{equal}                {BEGIN INITIAL; return token::TEQ;}                     /* = */
<INITIAL>{argname}              {BEGIN INITIAL; CAPTURE_TEXT(); return token::TARG;}    /* variable name and other symbols */
<INITIAL>{short_arg_pref}       {BEGIN SHORT_ARG_IN;}
<INITIAL>[[:blank:]]*           {BEGIN INITIAL;}
<INITIAL>.                      {BEGIN INITIAL; return token::TERR;}                    /* SYNTAX ERROR */

%%
