%{
#include "tp.tab.h"
#include <stdlib.h>
#include <stdio.h>
int text_to_int(const char *);
%}

%option noyywrap

%%
[1-9][0-9]*|"0" { yylval.entier = text_to_int(yytext); return INT; }
"("|")"|"+"|"-"|"*"|"/" { return yytext[0]; }
\n { }
. { }
%%

// converti un caractère en entier
int text_to_int(const char *s) {
  int k = atoi(s);
  return k;
}
