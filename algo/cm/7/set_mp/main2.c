//  Affiche sur la sortie standard :
//  - le nombre de mots différents lus sur l'entrée standard ;
//  - la liste de ces mots, dans l'ordre lexicographique.
//  Limitations :
//  - les mots sont obtenus par lecture sur l'entrée des suites consécutives
//    de longueur maximale mais majorée WORD_LENGTH_MAX de caractères qui ne
//    sont pas de la catégorie isspace ;
//  - toute suite de tels caractères de longueur strictement supérieure à
//    WORD_LENGTH_MAX se retrouve ainsi découpée en plusieurs mots.
//  Attention ! Les trois points suivants sont à retravailler. Les laisser en
//    l'état est contraire aux exigences prônées :
//  - l'avertissement qui figure ligne 32 est une nuisance si le mot lu a
//    exactement la longueur WORD_LENGTH_MAX ;
//  - l'allocation qui figure ligne 34 n'est pas testée ;
//  - les chaines allouées à la ligne 34 ne sont pas désallouées.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set_mp.h"

#define STR(x)  #x
#define XSTR(x) STR(x)

#define WORD_LENGTH_MAX 31

int main(void) {
  set *s = set_empty((int (*)(const void *, const void *))strcmp);
  char w[WORD_LENGTH_MAX + 1];
  while (scanf("%" XSTR(WORD_LENGTH_MAX) "s", w) == 1) {
    if (strlen(w) == WORD_LENGTH_MAX) {
      fprintf(stderr, "*** Warning: Word '%s...' possibly sliced.\n", w);
    }
    char *t = malloc(strlen(w) + 1);
    strcpy(t, w);
    set_add(s, t);
  }
  printf("%zu\n", set_card(s));
  while (!set_is_empty(s)) {
    char *t = (char *) set_min(s);
    printf("%s\n", t);
    set_remove(s, t);
  }
  set_dispose(&s);
  return EXIT_SUCCESS;
}
