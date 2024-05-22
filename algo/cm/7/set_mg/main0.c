#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set_mg.h"

SET(setstr, char *, strcmp)

int main(int argc, char *argv[]) {
  setstr *s = setstr_empty();
  for (int k = 1; k < argc; ++k) {
    setstr_add(s, argv[k]);
  }
  printf("%zu\n\"%s\"\n", setstr_card(s), (char *) setstr_min(s));
  setstr_dispose(&s);
  return EXIT_SUCCESS;
}
