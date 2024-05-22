%{
#include <stdlib.h>
#include <stdio.h>
#include "types.h"
int yylex();
void yyerror(char const *);
void begin();
void end();
%}

%union {
  int entier;
  types_symbole t_synth;
}

%token NUM OR AND TRUE FALSE

%left OR
%left AND
%left TRUE FALSE 
%left '+' '-'
%left '*' '/'

%type<t_synth> EXPR
%type<entier> NUM

%start EXPR

%%
EXPR:
  EXPR '+' EXPR {
  	if ($1 == NUM_T && $3 == NUM_T) {
  		printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tadd ax,bx\n"
				"\tpush ax\n"
			); $$ = NUM_T;
  	} else { $$ = ERR_T; }
  }
  
  | EXPR '-' EXPR {
  	if ($1 == NUM_T && $3 == NUM_T) {
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tsub bx,ax\n"
				"\tpush bx\n"
			); $$ = NUM_T;
		} else { $$ = ERR_T; }
  }
  
  | EXPR '*' EXPR {
  	if ($1 == NUM_T && $3 == NUM_T) {
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tmul ax,bx\n"
				"\tpush ax\n"
			); $$ = NUM_T;
		} else { $$ = ERR_T; }
  }
  
  | EXPR '/' EXPR {
  	if ($1 == NUM_T && $3 == NUM_T) {
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tconst dx,division\n"
				"\tdiv bx,ax\n"
				"\tjmpe dx\n"
				"\tpush bx\n"
			); $$ = NUM_T;
		} else { $$ = ERR_T; }
  }
  
  | EXPR OR EXPR {
  	if ($1 == BOOL_T && $3 == BOOL_T) {
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tadd ax,bx\n"
				"\tpush ax\n"
			); $$ = BOOL_T;
		} else { $$ = ERR_T; }
  }
  
  | EXPR AND EXPR {
  	if ($1 == BOOL_T && $3 == BOOL_T) {
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tmul ax,bx\n"
				"\tpush ax\n"
			); $$ = BOOL_T;
		} else { $$ = ERR_T; }
  }
  
  | '(' EXPR ')' { $$ = $2; }
  
  | NUM {
    printf(
    	"\tconst ax,%d\n"
    	"\tpush ax\n",
    	$1
    ); $$ = NUM_T;
  }
  
  | TRUE {
  	printf(
  		"\tconst ax,1\n"
  		"\tpush ax\n"
  	); $$ = BOOL_T;
  }
  
  | FALSE {
  	printf(
  		"\tconst ax,0\n"
  		"\tpush ax\n"
  	); $$ = BOOL_T;
  }
;
%%

int main(void) {
	begin();
  
  yyparse();
  
  end();
  
  return EXIT_SUCCESS;
}

void yyerror(char const *s) {
  fprintf(stderr, "%s\n", s);
}

void begin() {
	printf(
  	"; Programme\n"
  	"\tconst ax,debut\n"
  	"\tjmp ax\n"
  	"; Définition des constantes\n"
  	":erreur\n"
  	"@string \"Division par ZÉRO\\n\"\n\n"
  	":nl\n"
  	"@string \"\\n\"\n\n"
  	"; Initialisation de la pile\n"
  	":debut\n"
  	"\tconst bp,pile\n"
  	"\tconst sp,pile\n"
  	"\tconst ax,2\n"
  	"\tsub sp,ax\n"
  );
}
 
void end() {
	printf(
  	"; Affichage du résultat\n"
  	"\tcp cx,sp\n"
  	"\tcallprintfd cx\n"
  	"\tconst ax,nl\n"
  	"\tcallprintfs ax\n"
  	"\tpop ax\n"
  	"\tend\n\n"
  	"; Sortie sur erreur pour la divison\n"
  	":division\n"
  	"\tconst ax,erreur\n"
  	"\tcallprintfs ax\n"
  	"\tpop ax\n"
  	"\tend\n\n"
  	"; Zone de pile\n"
  	":pile\n"
  	"@int 0\n"
  );
}
