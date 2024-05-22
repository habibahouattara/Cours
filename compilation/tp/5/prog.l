%{
#include <stdbool.h>
#include "types.h"
#include "prog.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
%}

%option noyywrap

%%
[[:digit:]]+                    { yylval.entier = strtol(yytext, NULL, 10); return NUMB; }
"("|")"|"+"|"-"|"*"|"/"|"="|";" { return yytext[0]; }
"int"                           { return INT; }
"output"                        { return OUTPUT; }
"bool"                          { return BOOL; }
"true"                          { yylval.boolean = 1; return TRUE; }
"false"                         { yylval.boolean = 0; return FALSE; }
"and"                           { return AND; }
"or"                            { return OR; }
"=="                            { return EQ; }
"not"                           { return NOT; }
"!="                            { return NEQ; }
"<"                             { return INF; }
"<="                            { return INFEQ; }
">"                             { return SUP; }
">="                            { return SUPEQ; }
[a-zA-Z_][a-zA-Z0-9_]*          { yylval.id = malloc(sizeof(char) * (strlen(yytext) + 1));
                                  strncpy(yylval.id, yytext, strlen(yytext));
                                  return ID;
                                }
.|\n                            {}
%%
