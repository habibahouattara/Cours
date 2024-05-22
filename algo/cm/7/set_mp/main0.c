#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set_mp.h"

int main(int argc, char *argv[]) {
  set *s = set_empty((int (*)(const void *, const void *))strcmp);
  for (int k = 1; k < argc; ++k) {
    set_add(s, argv[k]);
  }
  printf("%zu\n\"%s\"\n", set_card(s), (char *) set_min(s));
  set_dispose(&s);
  return EXIT_SUCCESS;
}
