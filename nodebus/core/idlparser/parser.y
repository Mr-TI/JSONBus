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
%token              TDATETIME       "DateTime"
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
%type <node>        FIELD ATTRIBUTE CONSTANT EXPRESSION METHOD METHOD_PREFIX METHOD_SUFFIX PARAMETERS PARAMETER PARAMETER_DIR
%type <node>        VALUE ATTRIBUTE_QUAL INTERFACE_PARENT

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

MODULE : MODULE_BEGIN MODULE_ELTS '}' ';'         {driver.packagePop();}
    ;

ENUM : TENUM TSYMBOL '{' SYMBOL_LIST '}' ';'      {driver.lastError = "enum definition not supported yet";YYABORT;}
    ;

STRUCT : TSTRUCT TSYMBOL '{' STRUCT_ELTS '}' ';'  {driver.lastError = "struct definition not supported yet";YYABORT;}
    ;

EXCEPTION : TEXCEPTION TSYMBOL '{' STRUCT_ELTS '}' ';'
                                                  {driver.lastError = "exception definition not supported yet";YYABORT;}
    ;

STRUCT_ELTS : STRUCT_ELTS FIELD                   {$$ = $1; $$->list().append($2->map());}
    | FIELD                                       {$$ = Node::newList($1->map());}
    ;

TYPEDEF : TTYPEDEF TYPE TSYMBOL                   {driver.lastError = "typedef definition not supported yet";YYABORT;}
    ;

SYMBOL_LIST : SYMBOL_LIST ',' SYMBOL              {$$ = $1; $$->list().append($3->val());}
    | SYMBOL                                      {$$ = Node::newList($1->val());}
    ;

SYMBOL : SYMBOL ':' ':' TSYMBOL                   {$$ = new Node($1->toString() + "::" + $4->toString());}
    | TSYMBOL                                     {$$ = $1;}
    ;

INTERFACE : TINTERFACE SYMBOL INTERFACE_PARENT '{' INTERFACE_ELTS '}' ';'
                                                  {}
    ;

INTERFACE_PARENT : ':' SYMBOL_LIST                {$$ = $2;}
    |                                             {$$ = Node::newList();}
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

RET_TYPE : TYPE                                   {$$ = $1;}
    | TVOID                                       {$$ = new Node(TYPE_VOID);}
    ;

TYPE : TOBJECT                                    {$$ = new Node(TYPE_ANY);}
    | TANY                                        {$$ = new Node(TYPE_ANY);}
    | TBOOLEAN                                    {$$ = new Node(TYPE_BOOLEAN);}
    | TOCTET                                      {$$ = new Node(TYPE_BYTE);}
    | TCHAR                                       {$$ = new Node(TYPE_BYTE);}
    | TWCHAR                                      {$$ = new Node(TYPE_BYTE);}
    | TUNSIGNED TSHORT                            {$$ = new Node(TYPE_UINT32);}
    | TSHORT                                      {$$ = new Node(TYPE_INT32);}
    | TUNSIGNED TLONG                             {$$ = new Node(TYPE_UINT32);}
    | TLONG                                       {$$ = new Node(TYPE_INT32);}
    | TUNSIGNED TLONG TLONG                       {$$ = new Node(TYPE_UINT64);}
    | TLONG TLONG                                 {$$ = new Node(TYPE_INT64);}
    | TFLOAT                                      {$$ = new Node(TYPE_DOUBLE);}
    | TDOUBLE                                     {$$ = new Node(TYPE_DOUBLE);}
    | TSTRING                                     {$$ = new Node(TYPE_STRING);}
    | TOCTET '[' ']'                              {$$ = new Node(TYPE_BYTEARRAY);}
    | TDATETIME                                   {$$ = new Node(TYPE_DATETIME);}
    | SEQUENCE                                    {driver.lastError = "sequence not supported yet";YYABORT;}
    ;

FIELD : TYPE SYMBOL '<' TNUMBER '>'               {driver.lastError = "array definition not supported";YYABORT;}
    | TYPE SYMBOL '[' TNUMBER ']'                 {driver.lastError = "array definition not supported";YYABORT;}
    | TYPE SYMBOL '[' ']'                         {driver.lastError = "array definition not supported";YYABORT;}
    | TYPE SYMBOL                                 {$$ = Node::newMap(NODE_KEY_RET_TYPE, $1->val()); $$->map().insert("n", $2->val());}
    ;

ATTRIBUTE : ATTRIBUTE_QUAL TATTRIBUTE FIELD ';'   {$$ = $3; $$->map().insert(NODE_KEY_WRITABLE, $1->val());}
    ;

ATTRIBUTE_QUAL : TREADONLY                        {$$ = new Node(false);}
    |                                             {$$ = new Node(true);}
    ;

CONSTANT : TCONST FIELD '=' EXPRESSION ';'        {$$ = $2; $$->map().insert(NODE_KEY_VALUE, $4->val());}
    ;

EXPRESSION : '(' EXPRESSION ')'                   {$$ = $2;}
    | EXPRESSION '+' EXPRESSION                   {$$ = op_plus($1->val(), $3->val());}
    | EXPRESSION '-' EXPRESSION                   {$$ = op_minus($1->val(), $3->val());}
    | EXPRESSION '*' EXPRESSION                   {$$ = op_mult($1->val(), $3->val());}
    | EXPRESSION '/' EXPRESSION                   {$$ = op_divid($1->val(), $3->val());}
    | EXPRESSION '%' EXPRESSION                   {$$ = op_rest($1->val(), $3->val());}
    | VALUE                                       {$$ = $1;}
    ;

VALUE : TVARIANT                                  {$$ = $1;}
    | TSYMBOL                                     {driver.lastError = "symbol in expression not supported yet";YYABORT;}
    ;

METHOD : METHOD_PREFIX RET_TYPE SYMBOL '(' PARAMETERS ')' METHOD_SUFFIX ';'
                                                  {}
    ;

METHOD_PREFIX : TONEWAY                           {driver.lastError = "unsupported oneway keyword";YYABORT;}
    |                                             {}
    ;

METHOD_SUFFIX : TRAISES '(' SYMBOL_LIST ')'
                                                  {driver.lastError = "exception raise not supported";YYABORT;}
    |                                             {}
    ;

PARAMETERS : PARAMETERS PARAMETER                 {$$ = $1; $$->list().append($2->val());}
    | PARAMETER                                   {$$ = Node::newList($1->val());}
    | TVOID                                       {$$ = Node::newList();}
    |                                             {$$ = Node::newList();}
    ;

PARAMETER : PARAMETER_DIR FIELD                   {$$ = $2; $$->map().insert(NODE_KEY_DIRECTION, $1->val());}
    ; 

PARAMETER_DIR : TIN                               {$$ = new Node('\x01');}
    | TOUT                                        {$$ = new Node('\x02');}
    | TINOUT                                      {$$ = new Node('\x03');}
    |                                             {$$ = new Node('\x01');}
    ;


%%
