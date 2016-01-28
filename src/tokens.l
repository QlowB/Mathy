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
