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

#include "ltype.h"
#include <parser.hh>
#include "scanner.h"
#include "driver.h"
#include <qt4/QtCore/QVariant>
#define yylex driver.scanner.yylex

%}

%token              TEND 0          "end of file"

%token <node>       TINCLUDE        "#include<...>"

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
%token              TOCTET          "octet"
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

%token <node>       TSTRINGVAL      "string value"
%token <node>       TSYMBOL         "symbol"
%token <node>       TNUMBER         "number value"
%token <node>       TINTVAL         "integer value"
%token <node>       TFLOATVAL       "float value"
%token <node>       TVARIANT        "variant"

%token              TSYNERRESC      "invalid escaped character"
%token              TSYNERRUNI      "invalid unicode character"
%token              TSYNERR         "invalid character"

%start DOCUMENT

%type <node>        DOCUMENT DOCUMENT_ELTS DOCUMENT_ELT MODULE_ELTS MODULE_ELT MODULE_BEGIN MODULE ENUM STRUCT EXCEPTION
%type <node>        STRUCT_ELTS TYPEDEF SYMBOL_LIST SYMBOL INTERFACE INTERFACE_ELTS INTERFACE_ELT SEQUENCE RET_TYPE TYPE
%type <node>        FIELD ATTRIBUTE CONSTANT EXPRESSION METHOD METHOD_PREFIX METHOD_SUFFIX PARAMETERS PARAMETER PARAMETER_INOUT
%type <node>        VALUE

%left	'+' '-'
%left   '*' '/' '%'

%language "C++"
%define namespace "idlparser"
%define parser_class_name "Parser"
/*
%define api.namespace {idlparser}
%define parser_class_name {Parser}
*/
%parse-param {idlparser::Driver &driver}

%error-verbose

%%

DOCUMENT : DOCUMENT_ELTS TEND                     {YYACCEPT;}
    ;

DOCUMENT_ELTS : DOCUMENT_ELTS DOCUMENT_ELT        {}
    | DOCUMENT_ELT                                {}
    ;

DOCUMENT_ELT : MODULE_ELT                         {}
    | TINCLUDE                                    {}
    ;

MODULE_ELTS : MODULE_ELTS MODULE_ELT              {}
    | MODULE_ELT                                  {}
    ;

MODULE_ELT : INTERFACE                            {}
    | MODULE                                      {}
    | ENUM                                        {}
    | STRUCT                                      {}
    | TYPEDEF                                     {}
    | EXCEPTION                                   {}
    ;

MODULE_BEGIN : TMODULE TSYMBOL '{'                {driver.packagePush($2->toString());}
    ;

MODULE : MODULE_BEGIN MODULE_ELTS '}' ';'
                                                  {driver.packagePop();}
    ;

ENUM : TENUM TSYMBOL '{' SYMBOL_LIST '}' ';'
                                                  {driver.lastError = "enum definition not supported yet";YYABORT;}
    ;

STRUCT : TSTRUCT TSYMBOL '{' STRUCT_ELTS '}' ';'
                                                  {driver.lastError = "struct definition not supported yet";YYABORT;}
    ;

EXCEPTION : TEXCEPTION TSYMBOL '{' STRUCT_ELTS '}' ';'
                                                  {driver.lastError = "exception definition not supported yet";YYABORT;}
    ;

STRUCT_ELTS : STRUCT_ELTS FIELD                   {}
    | FIELD                                       {}
    ;

TYPEDEF : TYPE TSYMBOL                            {driver.lastError = "typedef definition not supported yet";YYABORT;}
    ;

SYMBOL_LIST : SYMBOL_LIST ',' SYMBOL              {}
    | SYMBOL                                      {}
    ;

SYMBOL : SYMBOL ':' ':' TSYMBOL                   {$$ = new Node(QVariant($1->toString() + "::" + $4->toString()));}
    | TSYMBOL                                     {$$ = $1;}
    ;

INTERFACE : TINTERFACE SYMBOL ':' SYMBOL_LIST '{' INTERFACE_ELTS '}' ';'
                                                  {}
    | TINTERFACE SYMBOL '{' INTERFACE_ELTS '}' ';'
                                                  {}
    ;

INTERFACE_ELTS : INTERFACE_ELTS INTERFACE_ELT     {}
    | INTERFACE_ELT                               {}
    ;

INTERFACE_ELT : ATTRIBUTE                         {}
    | METHOD                                      {}
    | CONSTANT                                    {}
    ;

SEQUENCE : TSEQUENCE '<' TYPE '>'                 {driver.lastError = "sequence not supported yet";YYABORT;}
    | TSEQUENCE '<' TYPE ',' TNUMBER '>'          {driver.lastError = "sequence not supported yet";YYABORT;}
    ;

RET_TYPE : TYPE                                   {}
    | TVOID                                       {}
    ;

TYPE : TBOOLEAN                                   {}
    | TFLOAT                                      {}
    | TDOUBLE                                     {}
    | TCHAR                                       {}
    | TWCHAR                                      {}
    | TSHORT                                      {}
    | TUNSIGNED TSHORT                            {}
    | TLONG                                       {}
    | TUNSIGNED TLONG                             {}
    | TLONG TLONG                                 {}
    | TUNSIGNED TLONG TLONG                       {}
    | TSTRING                                     {}
    | TOBJECT                                     {}
    | TANY                                        {}
    | SEQUENCE                                    {}
    ;

FIELD : TYPE SYMBOL                               {}
    | TYPE SYMBOL '[' TNUMBER ']'                 {driver.lastError = "array definition not supported";YYABORT;}
    | TYPE SYMBOL '<' TNUMBER '>'                 {driver.lastError = "array definition not supported";YYABORT;}
    ;

ATTRIBUTE : TATTRIBUTE FIELD ';'                  {}
    | TREADONLY TATTRIBUTE FIELD ';'              {}
    ;

CONSTANT : TCONST FIELD '=' EXPRESSION ';'        {}
    ;

EXPRESSION : '(' EXPRESSION ')'                   {$$ = $2;}
    | EXPRESSION '+' EXPRESSION                   {$$ = op_plus($1->variant(), $3->variant());}
    | EXPRESSION '-' EXPRESSION                   {$$ = op_minus($1->variant(), $3->variant());}
    | EXPRESSION '*' EXPRESSION                   {$$ = op_mult($1->variant(), $3->variant());}
    | EXPRESSION '/' EXPRESSION                   {$$ = op_divid($1->variant(), $3->variant());}
    | EXPRESSION '%' EXPRESSION                   {$$ = op_rest($1->variant(), $3->variant());}
    | VALUE                                       {$$ = $1;}
    ;

VALUE : TVARIANT                                  {$$ = $1;}
    | TSYMBOL                                     {driver.lastError = "symbol in expression not supported yet";YYABORT;}
    ;

METHOD : METHOD_PREFIX RET_TYPE SYMBOL '(' PARAMETERS ')' METHOD_SUFFIX ';'
                                                  {}
    ;

METHOD_PREFIX : TONEWAY                           {}
    |                                             {}
    ;

METHOD_SUFFIX : TRAISES '(' SYMBOL_LIST ')'
                                                  {driver.lastError = "exception raise not supported";YYABORT;}
    |                                             {}
    ;

PARAMETERS : PARAMETERS PARAMETER                 {}
    | PARAMETER                                   {}
    | TVOID                                       {}
    |                                             {}
    ;

PARAMETER : PARAMETER_INOUT FIELD                 {}
    ; 

PARAMETER_INOUT : TIN                             {$$ = new Node(char(1));}
    | TOUT                                        {$$ = new Node(char(2));}
    | TINOUT                                      {$$ = new Node(char(3));}
    |                                             {$$ = new Node(char(1));}
    ;


%%
