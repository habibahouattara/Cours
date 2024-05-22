%{
#include "tp.tab.h"
#include <stdlib.h>
#include <stdio.h>
int yylex();
void yyerror(char const *);
%}

%union {
  int entier;
}

%token <entier> INT

%left '+' '-'
%left '*' '/'

%start EXPR

%%
EXPR:
  EXPR '+' EXPR {}
  | EXPR '-' EXPR {}
  | EXPR '*' EXPR {}
  | EXPR '/' EXPR {}
  | '(' EXPR ')' {}
  | INT {
    printf("entier:%d\n",$1);
  }
;
%%

int main(void) {
  yyparse();
  
  return EXIT_SUCCESS;
}

void yyerror(char const *s) {
  fprintf(stderr, "%s\n", s);
}
