#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set_mg.h"

//  SET_PRINT : se reporter à "set_mg.h" pour les notations. Génère la fonction
//    <settype>_print qui permet l'affichage du contenu de l'ensemble associé à
//    son paramètre de type <settype> *. L'identificateur <elem_print> désigne
//    une procédure d'affichage d'un élément du type <elemtype> sur la sortie
//    standard. Utilise les délimiteurs "{" et "}" pour l'ensemble et le
//    séparateur ", " pour les éléments
#define SET_PRINT(settype, elemtype, elem_print)                               \
  void settype ## _print(settype * s) {                                        \
    settype *s2 = settype ## _duplicate(s);                                    \
    printf("{");                                                               \
    while (!settype ## _is_empty(s2)) {                                        \
      elemtype x = settype ## _min(s2);                                        \
      settype ## _remove(s2, x);                                               \
      elem_print(x);                                                           \
      if (!settype ## _is_empty(s2)) {                                         \
        printf(", ");                                                          \
      }                                                                        \
    }                                                                          \
    printf("}");                                                               \
    settype ## _dispose(&s2);                                                  \
  }

//  int_cmp, int_print : fonction de comparaison et procédure d'affichage sur le
//    type int
static int int_cmp(int x1, int x2);
static void int_print(int x);

SET(setint, int, int_cmp)
SET_PRINT(setint, int, int_print)

//  int_cmp, int_print : procédure d'affichage sur le type const char *
static void str_print(const char *s);

SET(setstr, const char *, strcmp)
SET_PRINT(setstr, const char *, str_print)

int main(void) {
  {
    setint *s = setint_empty();
    setint_add(s, 23);
    setint_add(s, 10);
    setint_add(s, 2019);
    setint_print(s);
    printf("\n");
    setint_remove(s, 10);
    setint_remove(s, 11);
    setint_print(s);
    printf("\n");
#if 0
    for (int k = 0;; ++k) {
      setint_add(s, k);
    }
#endif
    setint_dispose(&s);
  }
  {
    setstr *s = setstr_empty();
    setstr_add(s, "vingt-trois");
    setstr_add(s, "dix");
    setstr_add(s, "deux-mille-dix-neuf");
    setstr_print(s);
    printf("\n");
    setstr_remove(s, "dix");
    setstr_remove(s, "onze");
    setstr_print(s);
    printf("\n");
    setstr_dispose(&s);
  }
  return EXIT_SUCCESS;
}

int int_cmp(int x1, int x2) {
  return (x1 > x2) - (x1 < x2);
}

void int_print(int x) {
  printf("%d", x);
}

void str_print(const char *s) {
  printf("%s", s);
}
