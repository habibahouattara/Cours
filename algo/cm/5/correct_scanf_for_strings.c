#include <stdlib.h>
#include <stdio.h>

//==============================================================================
#if 1

//  À PROSCRIRE ABSOLUMENT : DÉBORDEMENT DE ZONE MÉMOIRE

int main(void) {
  char s[11];
  if (scanf("%s", s) != 1) {
    return EXIT_FAILURE;
  }
  printf("%s\n", s);
  return EXIT_SUCCESS;
}

//==============================================================================
#elif 1

//  À REJETER ÉGALEMENT : NOMBRES MAGIQUES

int main(void) {
  char s[11];
  if (scanf("%10s", s) != 1) {
    return EXIT_FAILURE;
  }
  printf("%s\n", s);
  return EXIT_SUCCESS;
}

//==============================================================================
#elif 1

//  À UTILISER DORÉNAVANT

#define STR(s)  #s
#define XSTR(s) STR(s)

#define STRINGLEN_MAX 10

int main(void) {
  char s[STRINGLEN_MAX + 1];
  if (scanf("%" XSTR(STRINGLEN_MAX) "s", s) != 1) {
    return EXIT_FAILURE;
  }
  printf("%s\n", s);
  return EXIT_SUCCESS;
}

//==============================================================================
#endif
