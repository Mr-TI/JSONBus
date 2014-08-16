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

%type <node>        DOCUMENT DOCUMENT_ELTS DOCUMENT_ELT MODULE_ELTS MODULE_ELT MODULE_HEADER MODULE ENUM STRUCT EXCEPTION
%type <node>        STRUCT_ELTS TYPEDEF SYMBOL_LIST SYMBOL INTERFACE INTERFACE_ELTS INTERFACE_ELT SEQUENCE RET_TYPE TYPE
%type <node>        FIELD ATTRIBUTE CONSTANT EXPRESSION METHOD METHOD_HEADER METHOD_FOOTER PARAMETERS PARAMETER PARAMETER_DIR
%type <node>        VALUE ATTRIBUTE_QUAL INTERFACE_PARENT INTERFACE_HEADER

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

MODULE : MODULE_HEADER '{' MODULE_ELTS '}' ';'    {driver.blockEnd();}
    ;

MODULE_HEADER : TMODULE TSYMBOL                   {driver.blockBegin($2->toString());}
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

ENUM : TENUM TSYMBOL '{' SYMBOL_LIST '}' ';'      {driver.lastError = "enum definition not supported yet";YYABORT;}
    ;

STRUCT : TSTRUCT TSYMBOL '{' STRUCT_ELTS '}' ';'  {driver.lastError = "struct definition not supported yet";YYABORT;}
    ;

STRUCT_ELTS : STRUCT_ELTS FIELD                   {$$ = $1; $$->append($2->map());}
    | FIELD                                       {$$ = Node::newList($1->map());}
    ;

TYPEDEF : TTYPEDEF TYPE TSYMBOL                   {driver.lastError = "typedef definition not supported yet";YYABORT;}
    ;

EXCEPTION : TEXCEPTION TSYMBOL '{' STRUCT_ELTS '}' ';'
                                                  {driver.lastError = "exception definition not supported yet";YYABORT;}
    ;

SYMBOL_LIST : SYMBOL_LIST ',' SYMBOL              {$$ = $1; $$->append($3->val());}
    | SYMBOL                                      {$$ = Node::newList($1->val());}
    ;

SYMBOL : SYMBOL ':' ':' TSYMBOL                   {$$ = new Node($1->toString() + "::" + $4->toString());}
    | TSYMBOL                                     {$$ = $1;}
    ;

INTERFACE : INTERFACE_HEADER '{' INTERFACE_ELTS '}' ';'
                                                  {driver.blockEnd();}
    ;

INTERFACE_HEADER : TINTERFACE SYMBOL INTERFACE_PARENT
                                                  {driver.blockBegin($2->toString());}
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
    | TVOID                                       {$$ = new Node(VTYPE_VOID);}
    ;

TYPE : TOBJECT                                    {$$ = new Node(VTYPE_ANY);}
    | TANY                                        {$$ = new Node(VTYPE_ANY);}
    | TBOOLEAN                                    {$$ = new Node(VTYPE_BOOLEAN);}
    | TOCTET                                      {$$ = new Node(VTYPE_BYTE);}
    | TCHAR                                       {$$ = new Node(VTYPE_BYTE);}
    | TWCHAR                                      {$$ = new Node(VTYPE_BYTE);}
    | TUNSIGNED TSHORT                            {$$ = new Node(VTYPE_UINT32);}
    | TSHORT                                      {$$ = new Node(VTYPE_INT32);}
    | TUNSIGNED TLONG                             {$$ = new Node(VTYPE_UINT32);}
    | TLONG                                       {$$ = new Node(VTYPE_INT32);}
    | TUNSIGNED TLONG TLONG                       {$$ = new Node(VTYPE_UINT64);}
    | TLONG TLONG                                 {$$ = new Node(VTYPE_INT64);}
    | TFLOAT                                      {$$ = new Node(VTYPE_DOUBLE);}
    | TDOUBLE                                     {$$ = new Node(VTYPE_DOUBLE);}
    | TSTRING                                     {$$ = new Node(VTYPE_STRING);}
    | TOCTET '[' ']'                              {$$ = new Node(VTYPE_BYTEARRAY);}
    | TDATETIME                                   {$$ = new Node(VTYPE_DATETIME);}
    | SEQUENCE                                    {driver.lastError = "Warning: sequence not supported yet";YYERROR;}
    ;

FIELD : TYPE SYMBOL '<' TNUMBER '>'               {driver.lastError = "Error: array definition not supported";YYABORT;}
    | TYPE SYMBOL '[' TNUMBER ']'                 {driver.lastError = "Error: array definition not supported";YYABORT;}
    | TYPE SYMBOL '[' ']'                         {driver.lastError = "Error: array definition not supported";YYABORT;}
    | TYPE SYMBOL                                 {$$ = Node::newMap(NODE_KEY_DTYPE, $1->val())->insert(NODE_KEY_SNAME, $2->val());}
    ;

ATTRIBUTE : ATTRIBUTE_QUAL TATTRIBUTE FIELD ';'   {$$ = $3->insert(NODE_KEY_WRITABLE, $1->val())->insert(NODE_KEY_TYPE, NTYPE_ATTR);}
    ;

ATTRIBUTE_QUAL : TREADONLY                        {$$ = new Node(false);}
    |                                             {$$ = new Node(true);}
    ;

CONSTANT : TCONST FIELD '=' EXPRESSION ';'        {$$ = $2; $$->insert(NODE_KEY_VALUE, $4->val());}
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
    | TSYMBOL                                     {QVariant v; if (!driver.resolve($1->val().toString(), v))YYABORT;$$ = new Node(v);}
    ;

METHOD : METHOD_HEADER RET_TYPE SYMBOL '(' PARAMETERS ')' METHOD_FOOTER ';'
                                                  {$$ = Node::newMap(NODE_KEY_DTYPE, $2->val())->insert(NODE_KEY_SNAME, $3->val())->insert(NODE_KEY_PARAMS, $5->map().values());}
    ;

METHOD_HEADER : TONEWAY                           {driver.lastError = "Warning: unsupported oneway keyword";YYERROR;}
    |                                             {}
    ;

METHOD_FOOTER : TRAISES '(' SYMBOL_LIST ')'
                                                  {driver.lastError = "exception raise not supported";YYABORT;}
    |                                             {}
    ;

PARAMETERS : PARAMETERS PARAMETER                 {if (!param_add($$, $1, $2, driver.lastError)) YYABORT;}
    | PARAMETER                                   {$$ = Node::newMap($1->map()[NODE_KEY_SNAME].toString(), $1->map());}
    | TVOID                                       {$$ = Node::newMap();}
    |                                             {$$ = Node::newMap();}
    ;

PARAMETER : PARAMETER_DIR FIELD                   {$$ = $2->insert(NODE_KEY_DIRECTION, $1->val());}
    ; 

PARAMETER_DIR : TIN                               {$$ = new Node('\x01');}
    | TOUT                                        {$$ = new Node('\x02');}
    | TINOUT                                      {$$ = new Node('\x03');}
    |                                             {$$ = new Node('\x01');}
    ;


%%
