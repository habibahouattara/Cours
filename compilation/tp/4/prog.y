%{
#include <stdlib.h>
#include <stdio.h>
#include "types.h"

#define SIZETAB 255

int yylex();
void yyerror(char const *);
void begin();
void end();
int compteur(void);
void creation_etiquette(char *name, const char *format, int value);
liste_symboles LS = NULL;
%}

%union {
  int entier;
  types_symbole t_synth;
  char* id;
}

%type<t_synth> EXPR INSTR
%token<entier> NUM
%token<id> ID
%token OR AND NOT EQ DIFF INF INFEG SUP SUPEG
%token OUTPUT

%left OR
%left AND
%left INF INFEG SUP SUPEG TRUE FALSE
%left EQ DIFF
%left '+' '-'
%left '*' '/'
%right NOT '='

%start LINSTR

%%
LINSTR:
	LINSTR INSTR
	|
;

INSTR:
	AFFECTATION ';'
	| OUTPUT '(' EXPR ')' ';' {
		// affichage du résult avec les callprintfd (cf. methode end())
	}
;

AFFECTATION:
ID '=' {
	// vérifier si ID est dans la table des symboles
} EXPR {
	// storew ax
	// donner le type adéquat à $4
}
;

EXPR:
  EXPR '+' EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
  		$$ = ERR_T;
  	} else {
  		printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tadd ax,bx\n"
				"\tpush ax\n"
			); $$ = NUM_T;
  	}
  }
  
  | EXPR '-' EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
			$$ = ERR_T;
		} else {
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tsub bx,ax\n"
				"\tpush bx\n"
			); $$ = NUM_T;
		}
  }
  
  | EXPR '*' EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
			$$ = ERR_T;
		} else {
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tmul ax,bx\n"
				"\tpush ax\n"
			); $$ = NUM_T;
		}
  }
  
  | EXPR '/' EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
			$$ = ERR_T;
		} else {
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tconst dx,division\n"
				"\tdiv bx,ax\n"
				"\tjmpe dx\n"
				"\tpush bx\n"
			); $$ = NUM_T;
		}
  }
  
  | EXPR OR EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
			$$ = ERR_T;
		} else {
			int n = compteur();
			char debut[SIZETAB];
			creation_etiquette(debut,"debut_or_%u",n);
			char fin[SIZETAB];
			creation_etiquette(fin,"fin_or_%u",n);
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tadd ax,bx\n"
				"\tconst bx,0\n"
				"\tconst cx,%s\n"
				"\tcmp ax,bx\n"
				"\tjmpc cx\n"
				"\tconst ax,1\n"
				"\tpush ax\n"
				"\tconst ax,%s\n"
				"\tjmp ax\n"
				":%s\n"
				"\tconst ax,0\n"
				"\tpush ax\n"
				":%s\n",debut,fin,debut,fin
			); $$ = BOOL_T;
		}
  }
  
  | EXPR DIFF EXPR {
  		if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
  		$$ = ERR_T;
  	} else {
  		int n = compteur();
  		char debut[SIZETAB];
			creation_etiquette(debut,"debut_diff_%u",n);
			char fin[SIZETAB];
			creation_etiquette(fin,"fin_diff_%u",n);
  		printf(
  			"\tpop ax\n"
        "\tpop bx\n"
        "\tcmp ax,bx\n"
        "\tjmpc cx\n"
        "\tconst cx,%s\n"
        "\tconst ax,1\n"
        "\tpush ax\n"
        "\tconst ax,%s\n"
        "\tjmp ax\n"
        ":%s\n"
        "\tconst ax,0\n"
        "\tpush ax\n"
        ":%s\n",debut,fin,debut,fin
  		); $$ = BOOL_T;
  	}
  }
  
  | EXPR EQ EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
  		$$ = ERR_T;
  	} else {
  		int n = compteur();
  		char debut[SIZETAB];
			creation_etiquette(debut,"debut_eq_%u",n);
			char fin[SIZETAB];
			creation_etiquette(fin,"fin_eq_%u",n);
  		printf(
  			"\tpop ax\n"
        "\tpop bx\n"
        "\tconst cx,%s\n"
        "\tcmp ax,bx\n"
        "\tjmpc cx\n"
        "\tconst ax,1\n"
        "\tpush ax\n"
        "\tconst dx,%s\n"
        "\tjmp dx\n"
        ":%s\n"
        "\tconst ax,0\n"
        "\tpush ax\n"
        ":%s\n",debut,fin,debut,fin
  		); $$ = BOOL_T;
  	}
  }
  
  | EXPR AND EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
			$$ = ERR_T;
		} else {
			int n = compteur();
			char debut[SIZETAB];
			creation_etiquette(debut,"debut_and_%u",n);
			char fin[SIZETAB];
			creation_etiquette(fin,"fin_and_%u",n);
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tmul ax,bx\n"
        "\tconst bx,%s\n"
        "\tcmp ax,bx\n"
        "\tjmpc cx\n"
        "\tconst ax,1\n"
				"\tpush ax\n"
        "\tconst ax,%s\n"
        "\tjmp ax\n"
        ":%s\n"
        "\tconst ax,0\n"
        "\tpush ax\n"
        ":%s\n",debut,fin,debut,fin
			); $$ = BOOL_T;
		}
  }
  
  | NOT EXPR {
  	if ($2 != BOOL_T) {
  		$$ = ERR_T;
  	} else {
  		int n = compteur();
  		char debut[SIZETAB];
			creation_etiquette(debut,"debut_not_%u",n);
			char fin[SIZETAB];
			creation_etiquette(fin,"fin_not_%u",n);
  		printf(
				"\tpop ax\n"
				"\tconst cx,%s\n"
				"\tconst bx,0\n"
				"\tcmp ax,bx\n"
				"\tjmpc cx\n"
				"\tconst ax,1\n"
				"\tpush ax\n"
				"\tconst ax,%s\n"
				"\tjmp ax\n"
				":%s\n"
				"\tconst ax,0\n"
				"\tpush ax\n"
				":%s\n",debut,fin,debut,fin
			); $$ = BOOL_T;
  	}
  }
  
  | EXPR SUPEG EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
			$$ = ERR_T;
		} else {
			int n = compteur();
			char debut[SIZETAB];
			creation_etiquette(debut,"debut_supeg_%u",n);
			char fin[SIZETAB];
			creation_etiquette(fin,"fin_supeg_%u",n);
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tconst cx,%s\n"
        "\tsless bx,ax\n"
        "\tjmpc cx\n"
        "\tconst ax,1\n"
        "\tpush ax\n"
        "\tconst ax,%s\n"
        "\tjmp ax\n"
        ":%s\n"
        "\tconst ax,0\n"
        "\tpush ax\n"
        ":%s\n",debut,fin,debut,fin
			); $$ = BOOL_T;
		}
  }
  
  | EXPR SUP EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
			$$ = ERR_T;
		} else {
			int n = compteur();
			char debut[SIZETAB];
			creation_etiquette(debut,"debut_sup_%u",n);
			char fin[SIZETAB];
			creation_etiquette(fin,"fin_sup_%u",n);
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tcp cx,bx\n"
				"\tconst dx,%s\n"
        "\tsless bx,ax\n"
        "\tjmpc dx\n"
        "\tcmp cx,ax\n"
        "\tjmpc dx\n"
        "\tconst ax,1\n"
        "\tpush ax\n"
        "\tconst ax,%s\n"
        "\tjmp ax\n"
        ":%s\n"
        "\tconst ax,0\n"
        "\tpush ax\n"
        ":%s\n",debut,fin,debut,fin
			); $$ = BOOL_T;
		}
  }
  
  | EXPR INFEG EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
			$$ = ERR_T;
		} else {
			int n = compteur();
			char debut[SIZETAB];
			creation_etiquette(debut,"debut_infeg_%u",n);
			char fin[SIZETAB];
			creation_etiquette(fin,"fin_infeg_%u",n);
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tcp cx,bx\n"
				"\tconst dx,%s\n"
        "\tsless bx,ax\n"
        "\tjmpc dx\n"
        "\tcmp cx,ax\n"
        "\tjmpc dx\n"
        "\tconst ax,1\n"
        "\tpush ax\n"
        "\tconst ax,%s\n"
        "\tjmp ax\n"
        ":%s\n"
        "\tconst ax,0\n"
        "\tpush ax\n"
        ":%s\n",debut,fin,debut,fin
			); $$ = BOOL_T;
		}
  }
  
  | EXPR INF EXPR {
  	if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
			$$ = ERR_T;
		} else {
			int n = compteur();
			char debut[SIZETAB];
			creation_etiquette(debut,"debut_inf_%u",n);
			char fin[SIZETAB];
			creation_etiquette(fin,"fin_inf_%u",n);
			printf(
				"\tpop ax\n"
				"\tpop bx\n"
				"\tcp cx,bx\n"
				"\tconst dx,%s\n"
        "\tsless bx,ax\n"
        "\tjmpc dx\n"
        "\tcmp cx,ax\n"
        "\tjmpc dx\n"
        "\tconst ax,1\n"
        "\tpush ax\n"
        "\tconst ax,%s\n"
        "\tjmp ax\n"
        ":%s\n"
        "\tconst ax,0\n"
        "\tpush ax\n"
        ":%s\n",debut,fin,debut,fin
			); $$ = BOOL_T;
		}
  }
  
  | '(' EXPR ')' { $$ = $2; }
  
  | NUM {
  	printf(
	  	"\tconst ax,%d\n"
	  	"\tpush ax\n",$1
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
  
  | ID {
  	liste_symboles *sym = inTS($1,ls);
  	if (sym == NULL) {
  		fprintf(stderr, "*** erreur: %s n'existe pas ***\n",$1);
  		$$ = UKN_T;
  	} else {
  		// mettre $1 dans la table
  	}
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

int compteur(void) {
	static int n = 0;
	++n;
	return n;
}

void creation_etiquette(char *name, const char *format, int value) {
	sprintf(name,format,value);
}
