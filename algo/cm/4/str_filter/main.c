#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "str_filter.h"

int main(void) {
  char *s = str_filter("0a1b2c3d4e5f6g789,;:!?", ispunct);
  if (s == NULL) {
    fprintf(stderr, "Heap error\n");
    exit(EXIT_FAILURE);
  }
  printf("\"%s\"\n", s);
  free(s);
  return EXIT_SUCCESS;
}
