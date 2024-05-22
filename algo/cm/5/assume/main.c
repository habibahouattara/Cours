#include <stdlib.h>
#include <stdio.h>

#define TOTO /* C'est l'histoire de Toto qui...
              */\
  1
#define TITI TJTJ
#define TJTJ TKTK
#define TKTK 2

#if 1

#include <assert.h>

int main(void) {
  assert(TOTO == TITI);
  return EXIT_SUCCESS;
}

#elif 1

#ifdef ASSUME_EXPR_EXPANSION
#undef ASSUME_EXPR_EXPANSION
#endif

#include "assume.h"

int main(void) {
  ASSUME(TOTO == TITI, "Gros minet !");
  return EXIT_SUCCESS;
}

#elif 1

#ifdef ASSUME_EXPR_EXPANSION
#undef ASSUME_EXPR_EXPANSION
#endif

#define ASSUME_EXPR_EXPANSION 1
#include "assume.h"

int main(void) {
  ASSUME(TOTO == TITI, "Gros minet !");
  return EXIT_SUCCESS;
}

#endif
