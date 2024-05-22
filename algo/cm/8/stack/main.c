//==============================================================================
#if 1

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int main(int argc, char *argv[]) {
  stack *s = stack_empty();
  for (int k = 1; k < argc; ++k) {
    stack_push(s, argv[k]);
  }
  while (!stack_is_empty(s)) {
    printf("%zu\t%s\n", stack_height(s), (char *) stack_top(s));
    stack_pop(s);
  }
  stack_dispose(&s);
  return EXIT_SUCCESS;
}

//==============================================================================
#elif 1

//  Affiche sur la sortie standard :
//  - le nombre de mots lus sur l'entrée standard ;
//  - la liste de ces mots dans l'ordre inverse de celui de leur lecture, chaque
//    mot étant précédé de son numéro d'ordre ;
//  - zéro.
//  Limitations :
//  - les mots sont obtenus par lecture sur l'entrée des suites consécutives
//    de longueur maximale mais majorée WORD_LENGTH_MAX de caractères qui ne
//    sont pas de la catégorie isspace ;
//  - toute suite de tels caractères de longueur strictement supérieure à
//    WORD_LENGTH_MAX se retrouve ainsi découpée en plusieurs mots.
//  Attention ! Le point suivant est à retravailler. Le laisser en l'état est
//    contraire aux exigences prônées :
//  - l'avertissement qui figure ligne 58 est une nuisance si le mot lu a
//    exactement la longueur WORD_LENGTH_MAX.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"

#define STR(s)  #s
#define XSTR(s) STR(s)

#define WORD_LENGTH_MAX 31

int main(void) {
  stack *s = stack_empty();
  if (s == NULL) {
    goto error_capacity;
  }
  char w[WORD_LENGTH_MAX + 1];
  while (scanf("%" XSTR(WORD_LENGTH_MAX) "s", w) == 1) {
    if (strlen(w) == WORD_LENGTH_MAX) {
      fprintf(stderr, "*** Warning: Word '%s...' possibly sliced.\n", w);
    }
    char *t = malloc(strlen(w) + 1);
    if (t == NULL) {
      goto error_capacity;
    }
    strcpy(t, w);
    if (stack_push(s, t) == NULL) {
      free(t);
      goto error_capacity;
    }
  }
  printf("%zu\n", stack_height(s));
  while (!stack_is_empty(s)) {
    char *t = (char *) stack_pop(s);
    printf("%zu\t%s\n", stack_height(s), t);
    free(t);
  }
  printf("%zu\n", stack_height(s));
  int r = EXIT_SUCCESS;
  goto dispose;
error_capacity:
  fprintf(stderr, "*** Error: Ask wizard to enlarge me.\n");
  r = EXIT_FAILURE;
  goto dispose;
dispose:
  stack_dispose(&s);
  return r;
}

#endif
