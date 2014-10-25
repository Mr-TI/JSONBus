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

%{

#include "globals.h"
#include <parser.hh>
#include "scanner.h"
#include "driver.h"
#include "routines.h"
#define yylex driver.scanner.yylex

using namespace std;

%}

%union {
  string_t *str;
  variant_t *node;
  array_t *array;
  object_t *object;
}

%token              TEND 0          "the end of file"

%token              TOBJBEGIN       "object begin '{'"
%token              TOBJEND         "object end '}'"
%token              TARRBEGIN       "array begin '['"
%token              TARREND         "array end ']'"
%token              TMEMBERSEP      "object field separator ':'"
%token              TELEMENTSEP     "object/array element separator ','"
%token <str>        TSTRING         "string"
%token <node>       TVARIANT        "variant"

%token              TSYNERRESC      "invalid escaped character"
%token              TSYNERRUNI      "invalid unicode character"
%token              TSYNERR         "invalid character"

%start ROOT

%type <object>      MEMBERS
%type <array>       ELEMENTS
%type <node>        ROOT VARIANT

%language "C++"
%define namespace "jsonparser"
%define parser_class_name "Parser"
/*
%define api.namespace {jsonparser}
%define parser_class_name {Parser}
*/
%parse-param {Driver &driver}

%destructor { delete $$; } TSTRING TVARIANT
%destructor { delete $$; } MEMBERS
%destructor { delete $$; } ELEMENTS
%destructor { delete $$; } ROOT VARIANT

%error-verbose

%%

ROOT : VARIANT                          {$$ = $1; driver.result->setValue(*$1); YYACCEPT;}
    | TEND                              {$$ = NULL; driver.result = NULL; YYACCEPT;}
    ;

VARIANT : TOBJBEGIN MEMBERS TOBJEND     {$$ = r::map2variant($2);}
    | TARRBEGIN ELEMENTS TARREND        {$$ = r::list2variant($2);}
    | TOBJBEGIN TOBJEND                 {$$ = r::map2variant(new object_t());}
    | TARRBEGIN TARREND                 {$$ = r::list2variant(new array_t());}
    | TSTRING                           {$$ = r::string2variant($1);}
    | TVARIANT                          {$$ = $1;}
    ;

MEMBERS : MEMBERS TELEMENTSEP
        TSTRING TMEMBERSEP VARIANT       {$$ = r::mapAppendVariant($1, $3, $5);}
    | TSTRING TMEMBERSEP VARIANT         {$$ = r::mapAppendVariant(NULL, $1, $3);}
    ;

ELEMENTS : ELEMENTS TELEMENTSEP VARIANT {$$ = r::listAppendVariant($1, $3);}
    | VARIANT                           {$$ = r::listAppendVariant(NULL, $1);}
    ;

%%
