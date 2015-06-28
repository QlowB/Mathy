%{
#include <string>
#include "Node.h"
#include "parser.h"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
bool end_of_file = false;
extern "C" int yywrap() { end_of_file = true; return 1; }
%}

%%

[ \t]                   ;
[ ][_][\n]              ; /* line break */
\n                      yyterminate();
[0-9]+\.[0-9]*          SAVE_TOKEN; return TREAL;
[0-9]+                  SAVE_TOKEN; return TINTEGER;

"+"                     return TOKEN(TPLUS);
"-"                     return TOKEN(TMINUS);
"*"                     return TOKEN(TMUL);
"/"                     return TOKEN(TDIV);
"^"                     return TOKEN(TPOW);
"mod"                   return TOKEN(TMOD);
"%"                     return TOKEN(TMOD);

"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"."                     return TOKEN(TDOT);
","                     return TOKEN(TCOMMA);
":"                     return TOKEN(TCOLON);

[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return TIDENTIFIER;

";"                     yyterminate();
[\0\-1]                 yyterminate();
.                       printf("Unknown token!\n"); yyterminate();

%%
