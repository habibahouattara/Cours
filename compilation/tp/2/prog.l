%{
#include "prog.tab.h"
#include <stdlib.h>
#include <stdio.h>
%}

%option noyywrap

%%
[1-9][0-9]*|"0" { yylval.entier = strtol(yytext,NULL,10); return NUM; }
"("|")"|"+"|"-"|"*"|"/" { return yytext[0]; }
\n { }
. { }
%%
