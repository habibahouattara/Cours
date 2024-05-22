//  Affiche sur la sortie standard la liste des différents mots lus sur
//    l'entrée standard, chaque mot étant précédé de son nombre d'occurrences.
//  Limitations :
//  - les mots sont obtenus par lecture sur l'entrée des suites consécutives
//    de longueur maximale mais majorée WORD_LENGTH_MAX de caractères qui ne
//    sont pas de la catégorie isspace ;
//  - toute suite de tels caractères de longueur strictement supérieure à
//    WORD_LENGTH_MAX se retrouve ainsi découpée en plusieurs mots.
//  Attention ! Le point suivant est à retravailler. Le laisser en l'état est
//    contraire aux exigences prônées :
//  - l'avertissement qui figure ligne 38 est une nuisance si le mot lu a
//    exactement la longueur WORD_LENGTH_MAX.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "holdall.h"

#define STR(s)  #s
#define XSTR(s) STR(s)

#define WORD_LENGTH_MAX 31

//  scptr_display : affiche sur la sortie standard *cptr, le caractère
//    tabulation, la chaine pointée par s et la fin de ligne. Renvoie zéro en
//    cas de succès, une valeur non nulle en cas d'échec
static int scptr_display(const char *s, const size_t *cptr);

//  rfree : libère la zone mémoire pointée par ptr et renvoie zéro
static int rfree(void *ptr);

int main(void) {
  table *t = table_empty((int (*)(const void *, const void *))strcmp);
  holdall *has = holdall_empty();
  holdall *hacptr = holdall_empty();
  if (t == NULL || has == NULL || hacptr == NULL) {
    goto error_capacity;
  }
  char w[WORD_LENGTH_MAX + 1];
  while (scanf("%" XSTR(WORD_LENGTH_MAX) "s", w) == 1) {
    if (strlen(w) == WORD_LENGTH_MAX) {
      fprintf(stderr, "*** Warning: Word '%s...' possibly sliced.\n", w);
    }
    size_t *cptr = (size_t *) table_search(t, w);
    if (cptr != NULL) {
      *cptr += 1;
    } else {
      char *s = malloc(strlen(w) + 1);
      if (s == NULL) {
        goto error_capacity;
      }
      strcpy(s, w);
      if (holdall_put(has, s) != 0) {
        free(s);
        goto error_capacity;
      }
      cptr = malloc(sizeof *cptr);
      if (cptr == NULL) {
        goto error_capacity;
      }
      *cptr = 1;
      if (holdall_put(hacptr, cptr) != 0) {
        free(cptr);
        goto error_capacity;
      }
      if (table_add(t, s, cptr) == NULL) {
        goto error_capacity;
      }
    }
  }
  fprintf(stderr, "--- Info: Number of distinct words: %zu.\n",
      holdall_count(has));
  if (holdall_apply_context(has, (int (*)(void *, void *))scptr_display,
      (void *(*)(void *, void *))table_search, t) != 0) {
    goto error_write;
  }
  int r = EXIT_SUCCESS;
  goto dispose;
error_capacity:
  fprintf(stderr, "*** Error: Not enough memory.\n");
  r = EXIT_FAILURE;
  goto dispose;
error_write:
  fprintf(stderr, "*** Error: A write error occurs.\n");
  r = EXIT_FAILURE;
  goto dispose;
dispose:
  table_dispose(&t);
  if (has != NULL) {
    holdall_apply(has, rfree);
  }
  holdall_dispose(&has);
  if (hacptr != NULL) {
    holdall_apply(hacptr, rfree);
  }
  holdall_dispose(&hacptr);
  return r;
}

int scptr_display(const char *s, const size_t *cptr) {
  return printf("%zu\t%s\n", *cptr, s) < 0;
}

int rfree(void *ptr) {
  free(ptr);
  return 0;
}