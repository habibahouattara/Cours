%{
#include <stdlib.h>
#include <stdio.h>
int yylex();
void yyerror(char const *);
void start();
void end();
%}

%union {
  int entier;
}

%token <entier> NUM

%left '+' '-'
%left '*' '/'

%start EXPR

%%
EXPR:
  EXPR '+' EXPR {
  	printf(
  		"\tpop ax\n"
  		"\tpop bx\n"
  		"\tadd ax,bx\n"
  		"\tpush ax\n"
  	);
  }
  | EXPR '-' EXPR {
  	printf(
  		"\tpop ax\n"
  		"\tpop bx\n"
  		"\tsub bx,ax\n"
  		"\tpush bx\n"
  	);
  }
  | EXPR '*' EXPR {
  	printf(
  		"\tpop ax\n"
  		"\tpop bx\n"
  		"\tmul ax,bx\n"
  		"\tpush ax\n"
  	);
  }
  | EXPR '/' EXPR {
  	printf(
  		"\tpop ax\n"
  		"\tpop bx\n"
  		"\tconst dx,division\n"
  		"\tdiv bx,ax\n"
  		"\tjmpe dx\n"
  		"\tpush bx\n"
  	);
  }
  | '(' EXPR ')' {}
  | NUM {
    printf(
    	"\tconst ax,%d\n"
    	"\tpush ax\n",
    	$1
    );
  }
;
%%

int main(void) {
	start();
  
  yyparse();
  
  end();
  
  return EXIT_SUCCESS;
}

void yyerror(char const *s) {
  fprintf(stderr, "%s\n", s);
}

void start() {
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
