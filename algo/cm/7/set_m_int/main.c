#include <stdlib.h>
#include <stdio.h>
#include "set_m_int.h"

//  set_print : affiche le contenu de l'ensemble associé à s. Utilise les
//    délimiteurs "{" et "}" pour l'ensemble et le séparateur ", " pour les
//    éléments
static void set_print(const set *s);

#define ADD(s, x)                                                              \
  printf("--- ajout de %d\n", x);                                              \
  set_add(s, x)

#define REMOVE(s, x)                                                           \
  printf("--- retrait de %d\n", x);                                            \
  set_remove(s, x)

#define CHECK_IN(s, x)  \
  printf("--- présence %d : %s\n", x, set_is_in(s, x) ? "oui" : "non")

#define CHECK_CARD_MIN(s) \
  printf("--- cardinal : %zu, minimum : %d\n", set_card(s), set_min(s))

#define PRINT(s)                                                               \
  set_print(s);                                                                \
  printf("\n")

int main(void) {
  set *s = set_empty();
  PRINT(s);
  ADD(s, 23);
  ADD(s, 10);
  ADD(s, 2019);
  PRINT(s);
  REMOVE(s, 10);
  PRINT(s);
  CHECK_IN(s, 10);
  CHECK_IN(s, 666);
  CHECK_IN(s, 2019);
  CHECK_CARD_MIN(s);
  REMOVE(s, set_min(s));
  PRINT(s);
#if 1
  for (int k = 1;; ++k) {
    ADD(s, k);
  }
#endif
  set_dispose(&s);
  return EXIT_SUCCESS;
}

void set_print(const set *s) {
  set *s2 = set_duplicate(s);
  printf("{");
  while (!set_is_empty(s2)) {
    int x = set_min(s2);
    set_remove(s2, x);
    printf("%d", x);
    if (!set_is_empty(s2)) {
      printf(", ");
    }
  }
  printf("}");
  set_dispose(&s2);
}
