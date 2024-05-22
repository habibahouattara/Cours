%{
#include "types.h"
#include "prog.tab.h"
#include <stdlib.h>
#include <stdio.h>

char *recopier(char *s);
%}

%option noyywrap

%%
[1-9][0-9]*|"0" 				{ yylval.entier = strtol(yytext,NULL,10); return NUM; }
"("|")"|"+"|"-"|"*"|"/"|"="|";" { return yytext[0]; }
"true" 									{ return TRUE; }
"false" 								{ return FALSE; }
"and" 									{ return AND; }
"or" 										{ return OR; }
"not"										{ return NOT; }
"=="										{ return EQ; }
"!="										{ return DIFF; }
"<"											{ return INF; }
"<="										{ return INFEG; }
">"											{ return SUP; }
">="										{ return SUPEG; }
"output"								{ return OUTPUT; }
[a-zA-Z_][a-zA-Z_0-9]*  { yylval.id = recopier(yytext); return ID; }
\n 											{ }
. 											{ }
%%

char *recopier(char *s) {
	char *id = malloc((yylen + 1) * sizeof(char));
	strcpy(id,s);
}
