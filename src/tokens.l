/* =============================================================================
//
// This file is part of the Mathy computer algebry system.
//
// Copyright (C) 2015-2016 Nicolas Winkler
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ===========================================================================*/

%{
#include <string>
#include "Node.h"
#include "parser.h"

#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)

bool end_of_file = false;
extern "C" int yywrap() { end_of_file = true; return 1; }
extern void yyerror(const char *s);
%}

%%

[ \t]                   ;
[ ][_][\n]              ; /* line break */
\n                      yyterminate();
[0-9]+\.[0-9]*          SAVE_TOKEN; return TOKEN_REAL;
[0-9]+                  SAVE_TOKEN; return TOKEN_INTEGER;

"+"                     return TOKEN(TOKEN_PLUS);
"-"                     return TOKEN(TOKEN_MINUS);
"*"                     return TOKEN(TOKEN_MUL);
"/"                     return TOKEN(TOKEN_DIV);
"^"                     return TOKEN(TOKEN_POW);
"mod"                   return TOKEN(TOKEN_MOD);
"%"                     return TOKEN(TOKEN_MOD);
":="                    return TOKEN(TOKEN_ASSIGNMENT);

"("                     return TOKEN(TOKEN_LPAREN);
")"                     return TOKEN(TOKEN_RPAREN);
"{"                     return TOKEN(TOKEN_LBRACE);
"}"                     return TOKEN(TOKEN_RBRACE);
"."                     return TOKEN(TOKEN_DOT);
","                     return TOKEN(TOKEN_COMMA);
":"                     return TOKEN(TOKEN_COLON);

[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return TOKEN_IDENTIFIER;

";"                     yyterminate();
[\0\-1]                 yyterminate();
.                       return TOKEN_ERROR;

%%
