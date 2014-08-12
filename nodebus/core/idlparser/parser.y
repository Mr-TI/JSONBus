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

#include "globals.h"
#include <parser.hh>
#include "scanner.h"
#include "driver.h"
#include "routines.h"
#define yylex driver.scanner.yylex

using namespace std;

%}

%union {
  string_t str;
  variant_t node;
  variant_list_t array;
  variant_map_t map;
}

%token              TEND 0          "end of file"

%token              TINCLUDE        "#include<...>"

%token              TMODULE         "module"
%token              TINTERFACE      "interface"

%token              TVOID           "void"
%token              TBOOLEAN        "boolean"
%token              TCHAR           "char"
%token              TWCHAR          "wchar"
%token              TFLOAT          "float"
%token              TDOUBLE         "double"
%token              TLONG           "long"
%token              TSHORT          "short"
%token              TLONG           "octet"
%token              TSTRING         "string"
%token              TOBJECT         "Object"
%token              TANY            "any"

%token              TUNSIGNED       "unsigned"
%token              TCONST          "const"
%token              TREADONLY       "readonly"
%token              TATTRIBUTE      "attribute"

%token              TENUM           "enum"
%token              TSTRUCT         "struct"
%token              TUNION          "union"
%token              TSWITCH         "switch"
%token              TCASE           "case"
%token              TDEFAULT        "default"
%token              TONEWAY         "oneway"
%token              TIN             "in"
%token              TOUT            "out"
%token              TINOUT          "inout"
%token              TSEQUENCE       "sequence"
%token              TEXCEPTION      "exception"
%token              TRAISES         "raises"

%token              TTYPEDEF        "typedef"

%token              TBLOCKBEGIN     "block begin '{'"
%token              TBLOCKEND       "block end '}'"
%token              TARRAYBEGIN     "array begin '['"
%token              TARRAYEND       "array end ']'"
%token              TPARTHBEGIN     "parenthesis begin '('"
%token              TPARTHEND       "parenthesis end ')'"
%token              TLTHAN          "less than '<'"
%token              TGTHAN          "greater than '>'"
%token              TCOLON          "colon ':'"
%token              TSEMICOLON      "semicolon ';'"
%token              TCOMA           "coma ','"
%token <str>        TSTRINGVAL      "string"
%token <str>        TSYMBOL         "string"
%token <node>       TNUMBER         "number value"
%token <node>       TINTVAL         "integer value"
%token <node>       TFLOATVAL       "float value"
%token <node>       TVARIANT        "variant"

%token              TSYNERRESC      "invalid escaped character"
%token              TSYNERRUNI      "invalid unicode character"
%token              TSYNERR         "invalid character"

%start DOCUMENT

%type <object>      MEMBERS
%type <array>       ELEMENTS
%type <node>        DOCUMENT VARIANT

%language "C++"
%define namespace "idlparser"
%define parser_class_name "Parser"
/*
%define api.namespace {jsonparser}
%define parser_class_name {Parser}
*/
%parse-param {Driver &driver}

%error-verbose

%%

DOCUMENT : DOCUMENT_ELTS TEND                     {$$ = $1; driver.result = $1; }
    ;

DOCUMENT_ELTS : DOCUMENT_ELTS DOCUMENT_ELT        {}
    | DOCUMENT_ELT                                {}
    ;

DOCUMENT_ELT : MODULE_ELT
    | TINCLUDE
    ;

MODULE_ELTS : MODULE_ELTS MODULE_ELT
    | MODULE_ELT
    ;

MODULE_ELT : INTERFACE
    | MODULE
    | ENUM
    | STRUCT
    | TYPEDEF
    | EXCEPTION
    ;

MODULE : TMODULE TSYMBOL TBLOCKBEGIN MODULE_ELTS TBLOCKEND TSEMICOLON TSEMICOLON
    ;

ENUM : TENUM TSYMBOL TBLOCKBEGIN ENUM_ELTS TBLOCKEND TSEMICOLON TSEMICOLON
    ;

STRUCT : TSTRUCT TSYMBOL TBLOCKBEGIN STRUCT_ELTS TBLOCKEND TSEMICOLON
    ;

EXCEPTION : TSTRUCT TSYMBOL TBLOCKBEGIN STRUCT_ELTS TBLOCKEND TSEMICOLON
    ;

STRUCT_ELTS : STRUCT_ELTS FIELD
    | FIELD
    ;



SYMBOL_LIST : SYMBOL_LIST TCOMA TSYMBOL
    | TSYMBOL
    ;

INTERFACE : TINTERFACE TSYMBOL TCOLON SYMBOL_LIST TBLOCKBEGIN INTERFACE_ELTS TBLOCKEND TSEMICOLON
    | TINTERFACE TSYMBOL TBLOCKBEGIN INTERFACE_ELTS TBLOCKEND TSEMICOLON
    ;

INTERFACE_ELTS : INTERFACE_ELTS INTERFACE_ELT
    | INTERFACE_ELT
    ;

INTERFACE_ELT : ATTRIBUTE
    | METHOD
    ;

SEQUENCE : TSEQUENCE TLTHAN TYPE TGTHAN
    | TSEQUENCE TLTHAN TYPE TCOMA TNUMBER TGTHAN
    ;

RET_TYPE : TYPE
    | TVOID
    ;

TYPE : TBOOLEAN
    | TFLOAT
    | TDOUBLE
    | TCHAR
    | TWCHAR
    | TSHORT
    | TUNSIGNED TSHORT
    | TLONG
    | TUNSIGNED TLONG
    | TLONG TLONG
    | TUNSIGNED TLONG TLONG
    | TSTRING
    | TOBJECT
    | TANY
    | SEQUENCE
    ;

FIELD : TYPE TSYMBOL
    | TYPE TSYMBOL TARRAYBEGIN TNUMBER TARRAYEND
    | TYPE TSYMBOL TLTHAN TNUMBER TGTHAN
    ;

ATTRIBUTE : TATTRIBUTE FIELD
    | TREADONLY TATTRIBUTE FIELD
    ;

METHOD : METHOD_PREFIX RET_TYPE TSYMBOL TPARTHBEGIN TPARTHEND METHOD_SUFFIX TCOMA
    | METHOD_PREFIX RET_TYPE TSYMBOL TPARTHBEGIN PARAMETERS TPARTHEND METHOD_SUFFIX TCOMA
    ;

METHOD_PREFIX : TONEWAY
    |
    ;

METHOD_SUFFIX : TRAISES TPARTHBEGIN TSYMBOL TPARTHEND
    |
    ;

PARAMETERS : PARAMETERS PARAMETER
    | PARAMETER
    ;

PARAMETER : PARAMETER_INOUT FIELD
    ; 

PARAMETER_INOUT : TIN
    | TOUT
    | TINOUT
    |
    ;










%%
