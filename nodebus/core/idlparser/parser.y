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

%left   '+' '-'
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

MODULE_ELT : INTERFACE                            {$$ = $1;}
    | MODULE                                      {$$ = $1;}
    | ENUM                                        {$$ = $1;}
    | STRUCT                                      {$$ = $1;}
    | TYPEDEF                                     {$$ = $1;}
    | EXCEPTION                                   {$$ = $1;}
    ;

ENUM : TENUM TSYMBOL '{' SYMBOL_LIST '}' ';'      {driver.lastError = "enum definition not supported yet";YYABORT;}
    ;

STRUCT : TSTRUCT TSYMBOL '{' STRUCT_ELTS '}' ';'  {driver.lastError = "struct definition not supported yet";YYABORT;}
    ;

STRUCT_ELTS : STRUCT_ELTS FIELD                   {$$ = $1; $$->append($2->map());}
    | FIELD                                       {$$ = (new NodeList())->append($1->map());}
    ;

TYPEDEF : TTYPEDEF TYPE TSYMBOL                   {driver.lastError = "typedef definition not supported yet";YYABORT;}
    ;

EXCEPTION : TEXCEPTION TSYMBOL '{' STRUCT_ELTS '}' ';'
                                                  {driver.lastError = "exception definition not supported yet";YYABORT;}
    ;

SYMBOL_LIST : SYMBOL_LIST ',' SYMBOL              {$$ = $1; $$->append($3->val());}
    | SYMBOL                                      {$$ = (new NodeList())->append($1->val());}
    ;

SYMBOL : SYMBOL ':' ':' TSYMBOL                   {$$ = new NodeVariant($1->toString() + "::" + $4->toString());}
    | TSYMBOL                                     {$$ = $1;}
    ;

INTERFACE : INTERFACE_HEADER '{' INTERFACE_ELTS '}' ';'
                                                  {driver.blockEnd();}
    ;

INTERFACE_HEADER : TINTERFACE SYMBOL INTERFACE_PARENT
                                                  {driver.blockBegin($2->toString());}
    ;

INTERFACE_PARENT : ':' SYMBOL_LIST                {$$ = $2;}
    |                                             {$$ = (new NodeList());}
    ;

INTERFACE_ELTS : INTERFACE_ELTS INTERFACE_ELT     {}
    | INTERFACE_ELT                               {}
    ;

INTERFACE_ELT : ATTRIBUTE                         {}
    | METHOD                                      {}
    | CONSTANT                                    {}
    ;

SEQUENCE : TSEQUENCE '<' TYPE '>'                 {driver.lastError = "sequence not supported";YYABORT;}
    | TSEQUENCE '<' TYPE ',' TNUMBER '>'          {driver.lastError = "sequence not supported";YYABORT;}
    ;

RET_TYPE : TYPE                                   {$$ = $1;}
    | TVOID                                       {$$ = new NodeVariant(VTYPE_VOID);}
    ;

TYPE : TOBJECT                                    {$$ = new NodeVariant(VTYPE_ANY);}
    | TANY                                        {$$ = new NodeVariant(VTYPE_ANY);}
    | TBOOLEAN                                    {$$ = new NodeVariant(VTYPE_BOOLEAN);}
    | TOCTET                                      {$$ = new NodeVariant(VTYPE_BYTE);}
    | TCHAR                                       {$$ = new NodeVariant(VTYPE_BYTE);}
    | TWCHAR                                      {$$ = new NodeVariant(VTYPE_BYTE);}
    | TUNSIGNED TSHORT                            {$$ = new NodeVariant(VTYPE_UINT32);}
    | TSHORT                                      {$$ = new NodeVariant(VTYPE_INT32);}
    | TUNSIGNED TLONG                             {$$ = new NodeVariant(VTYPE_UINT32);}
    | TLONG                                       {$$ = new NodeVariant(VTYPE_INT32);}
    | TUNSIGNED TLONG TLONG                       {$$ = new NodeVariant(VTYPE_UINT64);}
    | TLONG TLONG                                 {$$ = new NodeVariant(VTYPE_INT64);}
    | TFLOAT                                      {$$ = new NodeVariant(VTYPE_DOUBLE);}
    | TDOUBLE                                     {$$ = new NodeVariant(VTYPE_DOUBLE);}
    | TSTRING                                     {$$ = new NodeVariant(VTYPE_STRING);}
    | TOCTET '[' ']'                              {$$ = new NodeVariant(VTYPE_BYTEARRAY);}
    | TDATETIME                                   {$$ = new NodeVariant(VTYPE_DATETIME);}
    | SEQUENCE                                    {driver.lastError = "Warning: sequence not supported yet";YYERROR;}
    ;

FIELD : TYPE SYMBOL '<' TNUMBER '>'               {driver.lastError = "Error: array definition not supported";YYABORT;}
    | TYPE SYMBOL '[' TNUMBER ']'                 {driver.lastError = "Error: array definition not supported";YYABORT;}
    | TYPE SYMBOL '[' ']'                         {driver.lastError = "Error: array definition not supported";YYABORT;}
    | TYPE SYMBOL                                 {$$ = (new NodeMap())->insert(KNODE_DTYPE, $1->val())->insert(KNODE_SNAME, $2->val());}
    ;

ATTRIBUTE : ATTRIBUTE_QUAL TATTRIBUTE FIELD ';'   {$$ = $3->insert(KNODE_WRITABLE, $1->val())->insert(KNODE_TYPE, NTYPE_ATTR);}
    ;

ATTRIBUTE_QUAL : TREADONLY                        {$$ = new NodeVariant(false);}
    |                                             {$$ = new NodeVariant(true);}
    ;

CONSTANT : TCONST FIELD '=' EXPRESSION ';'        {$$ = $2; $$->insert(KNODE_VALUE, $4->val());}
    ;

EXPRESSION : '(' EXPRESSION ')'                   {$$ = $2;}
    | EXPRESSION '+' EXPRESSION                   {if (!opexec($$, '+', $1, $3, driver.lastError))YYABORT;}
    | EXPRESSION '-' EXPRESSION                   {if (!opexec($$, '-', $1, $3, driver.lastError))YYABORT;}
    | EXPRESSION '*' EXPRESSION                   {if (!opexec($$, '*', $1, $3, driver.lastError))YYABORT;}
    | EXPRESSION '/' EXPRESSION                   {if (!opexec($$, '/', $1, $3, driver.lastError))YYABORT;}
    | EXPRESSION '%' EXPRESSION                   {if (!opexec($$, '%', $1, $3, driver.lastError))YYABORT;}
    | VALUE                                       {$$ = $1;}
    ;

VALUE : TVARIANT                                  {$$ = $1;}
    | TSYMBOL                                     {$$ = $1;}
    ;

METHOD : METHOD_HEADER RET_TYPE SYMBOL '(' PARAMETERS ')' METHOD_FOOTER ';'
                                                  {$$ = (new NodeMap())->insert(KNODE_DTYPE, $2->val())->insert(KNODE_SNAME, $3->val())->insert(KNODE_PARAMS, $5->map());}
    ;

METHOD_HEADER : TONEWAY                           {driver.lastError = "Warning: unsupported oneway keyword and will be ignored";YYERROR;}
    |                                             {}
    ;

METHOD_FOOTER : TRAISES '(' SYMBOL_LIST ')'
                                                  {driver.lastError = "Warning: exception raise not supported and will be ignored";YYERROR;}
    |                                             {}
    ;

PARAMETERS : PARAMETERS PARAMETER                 {if ($1->appendParam($2, driver.lastError)) YYABORT; $$ = $1;}
    | PARAMETER                                   {$$ = new NodeParams(); $$->appendParam($1, driver.lastError);}
    | TVOID                                       {$$ = new NodeParams();}
    |                                             {$$ = new NodeParams();}
    ;

PARAMETER : PARAMETER_DIR FIELD                   {$$ = $2->insert(KNODE_DIRECTION, $1->val());}
    ; 

PARAMETER_DIR : TIN                               {$$ = new NodeVariant(PDIR_IN);}
    | TOUT                                        {$$ = new NodeVariant(PDIR_OUT);}
    | TINOUT                                      {$$ = new NodeVariant(PDIR_INOUT);}
    |                                             {$$ = new NodeVariant(PDIR_IN);}
    ;


%%
