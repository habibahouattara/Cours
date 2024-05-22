%{
#include "types.h"
#include "prog.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE_BUF 255

int yylex();
void yyerror(char const *s);
void startCodeAssembleur();
void endCodeAssembleur();

int n;
int ordre = 0;
table_symbole TS = NULL;
table_symbole sym = NULL;

int createNumLabel();
void createLabel(char *name, const char *format, int value);
char debut_label[SIZE_BUF];
char end_label[SIZE_BUF];
%}

%union {
  int entier;
  bool boolean;
  types_symbole type_sym;
  char *id;
}

%token<entier> NUMB
%token<id> ID
%token<boolean> BOOL

%token INT OUTPUT TRUE FALSE AND OR EQ NOT NEQ INF INFEQ SUP SUPEQ

%left OR
%left AND
%left EQ NEQ SUP SUPEQ INF INFEQ NOT
%left '+' '-'
%left '*' '/'
%right '='

%type<type_sym> EXPR

%start PROG

%%
PROG:
  LINSTR
;

LINSTR:
  LINSTR INSTR
  |
;

INSTR:
  AFF ';'
  | OUTPUT '(' EXPR ')' ';' {
    printf(
      "\tcp cx,sp\n"
      "\tcallprintfd cx\n"
      "\tconst ax,nl\n"
      "\tcallprintfs ax\n"
      "\tpop ax\n"
    );
  }
;

AFF:
  ID '=' {
    sym = inTS($1, TS);
    if (sym == NULL) {
      ordre += 1;
      TS = addSymbol($1, VLOC, ordre, TS);
      sym = TS;
    }
  } EXPR {
    if (sym->type_s == UKN_T) {
      sym->type_s = $4;
    }
    if (sym->type_s == $4) {
      printf(
        "\tpop ax\n"
        "\tconst bx,var:%s\n"
        "\tstorew ax,bx\n", $1
      );
    }
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
      );
      $$ = INT_T;
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
      );
      $$ = INT_T;
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
      );
      $$ = INT_T;
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
      );
      $$ = INT_T;
    }
  }
  
  | '(' EXPR ')' { $$ = $2; }
  
  | EXPR AND EXPR {
    if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
      $$ = ERR_T;
    } else {
      n = createNumLabel();
      createLabel(debut_label, "debut_label_and_%u", n);
      createLabel(end_label, "end_label_and_%u", n);
      printf(
        "\tpop ax\n"
        "\tpop bx\n"
        "\tmul ax,bx\n"
        "\tconst cx,0\n"
        "\tconst dx,%s\n"
        "\tcmp ax,cx\n"
        "\tjmpc dx\n"
        "\tconst ax,1\n"
        "\tpush ax\n"
        "\tconst cx,%s\n"
        "\tjmp cx\n"
        ":%s\n"
        "\tconst ax,0\n"
        "\tpush ax\n"
        ":%s\n", debut_label, end_label, debut_label, end_label
      );
      $$ = BOOL_T;
    }
  }
  
  | EXPR OR EXPR {
    if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
      $$ = ERR_T;
    } else {
      n = createNumLabel();
      createLabel(debut_label, "debut_label_or_%u", n);
      createLabel(end_label, "end_label_or_%u", n);
      printf(
        "\tpop ax\n"
        "\tpop bx\n"
        "\tadd ax,bx\n"
        "\tconst cx,0\n"
        "\tconst dx,%s\n"
        "\tcmp ax,cx\n"
        "\tjmpc dx\n"
        "\tconst ax,1\n"
        "\tpush ax\n"
        "\tconst cx,%s\n"
        "\tjmp cx\n"
        ":%s\n"
        "\tconst ax,0\n"
        "\tpush ax\n"
        ":%s\n", debut_label, end_label, debut_label, end_label
      );
      $$ = BOOL_T;
    }
  }
  
  | EXPR EQ EXPR {
    if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
      $$ = ERR_T;
    } else {
      n = createNumLabel();
      createLabel(debut_label, "debut_label_eq_%u", n);
      createLabel(end_label, "end_label_eq_%u", n);
      printf(
        "\tpop ax\n"
        "\tpop bx\n"
        "\tconst cx,%s\n"
        "\tcmp ax,bx\n"
        "\tjmpc cx\n"
        "\tconst dx,0\n"
        "\tpush dx\n"
        "\tconst bx,%s\n"
        "\tjmp bx\n"
        ":%s\n"
        "\tconst dx,1\n"
        "\tpush dx\n"
        ":%s\n", debut_label, end_label, debut_label, end_label
      );
      $$ = BOOL_T;
    }
  }
  
  | NOT EXPR {
    if ($2 == ERR_T || $2 != BOOL_T) {
      $$ = ERR_T;
    } else {
      n = createNumLabel();
      createLabel(debut_label, "debut_label_not_%u", n);
      createLabel(end_label, "end_label_not_%u", n);
      printf(
        "\tpop ax\n"
        "\tconst cx,%s\n"
        "\tconst bx,0\n"
        "\tcmp ax,bx\n"
        "\tjmpc cx\n"
        "\tpush bx\n"
        "\tconst bx,%s\n"
        "\tjmp bx\n"
        ":%s\n"
        "\tconst dx,1\n"
        "\tpush dx\n"
        ":%s\n", debut_label, end_label, debut_label, end_label
      );
      $$ = BOOL_T;
    }
  }
  
  | EXPR NEQ EXPR {
    if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
      $$ = ERR_T;
    } else {
      n = createNumLabel();
      createLabel(debut_label, "debut_label_neq_%u", n);
      createLabel(end_label, "end_label_neq_%u", n);
      printf(
        "\tpop ax\n"
        "\tpop bx\n"
        "\tconst cx,%s\n"
        "\tcmp ax,bx\n"
        "\tjmpc cx\n"
        "\tconst dx,1\n"
        "\tpush dx\n"
        "\tconst bx,%s\n"
        "\tjmp bx\n"
        ":%s\n"
        "\tconst dx,0\n"
        "\tpush dx\n"
        ":%s\n", debut_label, end_label, debut_label, end_label
      );
      $$ = BOOL_T;
    }
  }
  
  | EXPR INF EXPR {
    if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
      $$ = ERR_T;
    } else {
      n = createNumLabel();
      createLabel(debut_label, "debut_label_inf_%u", n);
      createLabel(end_label, "end_label_inf_%u", n);
      printf(
        "\tpop ax\n"
        "\tpop bx\n"
        "\tconst cx,%s\n"
        "\tcmp ax,bx\n"
        "\tjmpc cx\n"
        "\tconst dx,0\n"
        "\tpush dx\n"
        "\tconst bx,%s\n"
        "\tjmp bx\n"
        ":%s\n"
        "\tconst dx,1\n"
        "\tpush dx\n"
        ":%s\n", debut_label, end_label, debut_label, end_label
      );
      $$ = BOOL_T;
    }
  }
  
  | EXPR INFEQ EXPR {
    if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
      $$ = ERR_T;
    } else {
      n = createNumLabel();
      createLabel(debut_label, "debut_label_infeq_%u", n);
      createLabel(end_label, "end_label_infeq_%u", n);
      printf(
        "\tpop ax\n"
        "\tpop bx\n"
        "\tconst cx,%s\n"
        "\tcmp ax,bx\n"
        "\tjmpc cx\n"
        "\tconst dx,0\n"
        "\tpush dx\n"
        "\tconst bx,%s\n"
        "\tjmp bx\n"
        ":%s\n"
        "\tconst dx,1\n"
        "\tpush dx\n"
        ":%s\n", debut_label, end_label, debut_label, end_label
      );
      $$ = BOOL_T;
    }
  }

  | EXPR SUP EXPR {
    if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
      $$ = ERR_T;
    } else {
      n = createNumLabel();
      createLabel(debut_label, "debut_label_sup_%u", n);
      createLabel(end_label, "end_label_sup_%u", n);
      printf(
        "\tpop ax\n"
        "\tpop bx\n"
        "\tconst cx,%s\n"
        "\tcmp ax,bx\n"
        "\tjmpc cx\n"
        "\tconst dx,0\n"
        "\tpush dx\n"
        "\tconst bx,%s\n"
        "\tjmp bx\n"
        ":%s\n"
        "\tconst dx,1\n"
        "\tpush dx\n"
        ":%s\n", debut_label, end_label, debut_label, end_label
      );
      $$ = BOOL_T;
    }
  }

  | EXPR SUPEQ EXPR {
    if ($1 == ERR_T || $3 == ERR_T || $1 != $3) {
      $$ = ERR_T;
    } else {
      n = createNumLabel();
      createLabel(debut_label, "debut_label_supeq_%u", n);
      createLabel(end_label, "end_label_supeq_%u", n);
      printf(
        "\tpop ax\n"
        "\tpop bx\n"
        "\tconst cx,%s\n"
        "\tcmp ax,bx\n"
        "\tjmpc cx\n"
        "\tconst dx,0\n"
        "\tpush dx\n"
        "\tconst bx,%s\n"
        "\tjmp bx\n"
        ":%s\n"
        "\tconst dx,1\n"
        "\tpush dx\n"
        ":%s\n", debut_label, end_label, debut_label, end_label
      );
      $$ = BOOL_T;
    }
  }
  
  | TRUE {
    printf(
      "\tconst ax,1\n"
      "\tpush ax\n"
    );
    $$ = BOOL_T;
  }
  
  | FALSE {
    printf(
      "\tconst ax,0\n"
      "\tpush ax\n"
    );
    $$ = BOOL_T;
  }
  
  | NUMB {
    printf(
      "\tconst ax,%d\n"
      "\tpush ax\n",$1
    );
    $$ = INT_T;
  }
  
  | ID {
    sym = inTS($1, TS);
    if (sym == NULL) {
      fprintf(stderr, "Variable %s non défini dans la table des symboles\n", $1);
      exit(EXIT_FAILURE);
    } else {
      printf(
        "\tconst bx,var:%s\n"
        "\tloadw ax,bx\n"
        "\tpush ax\n", $1
      );
      $$ = sym->type_s;
    }
  }
;
%%

int main(void) {
  startCodeAssembleur();
  
  yyparse();
  
  endCodeAssembleur();
  
  return EXIT_SUCCESS;
}

void yyerror(char const *s) {
  fprintf(stderr, "%s\n", s);
}

void startCodeAssembleur() {
  printf(
    "; Programme\n"
    "\tconst ax,debut\n"
    "\tjmp ax\n"
    "; Définition des constantes\n"
    ":erreur\n"
    "@string \"Division par zéro\\n\"\n\n"
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

void endCodeAssembleur() {
  printf(
    "\tend\n"
    "; Variables\n"
    ":var:a\n"
    "@int 0\n"
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

table_symbole addSymbol(char *id, types_variable tv, size_t ordre, table_symbole ts) {
  table_symbole t = malloc(sizeof *t);
  if (t == NULL) {
    fprintf(stderr, "erreur d'allocation pour la table des symboles\n");
    exit(EXIT_FAILURE);
  }
  t->id = id;
  t->type_s = UKN_T;
  t->ordre = ordre;
  t->type_v = tv;
  t->next = ts;
  return t;
}

table_symbole inTS(char *id, table_symbole ts) {
  table_symbole t = ts;
  while (t != NULL) {
    if (strcmp(id, t->id) == 0) {
      return t;
    }
    t = t->next;
  }
  return NULL;
}

void freeFirstCell() {
  if (sym != NULL) {
    table_symbole t = sym->next;
    free((void *) sym->id);
    free(sym);
    sym = t;
  }
}

int createNumLabel() {
  static int n = 0;
  ++n;
  return n;
}

void createLabel(char *name, const char *format, int value) {
  sprintf(name, format, value);
}
