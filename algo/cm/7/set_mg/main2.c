#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sub2.h"

int main(int argc, char *argv[]) {
  setstr *s = setstr_empty();
  for (int k = 1; k < argc; ++k) {
    setstr_add(s, argv[k]);
  }
  setstr_print(s);
  printf("\n");
  setstr_dispose(&s);
  return EXIT_SUCCESS;
}
