#include <stdlib.h>
#include <stdio.h>
#include "str_scat.h"

int main(void) {
  char *s1 = str_scat('\0', "renvoie", "la", "concaténation", "des", "chaines",
          "sans", "séparateur", NULL);
  char *s2 = str_scat('*', "renvoie", "la", "concaténation", "des", "chaines",
          "avec", "séparateur", "étoilé", NULL);
  char *s3 = STR_SCAT('-', "14", "07", "1789");
  int err = (s1 == NULL || s2 == NULL || s3 == NULL);
  if (!err) {
    printf("|%s|\n", s1);
    printf("|%s|\n", s2);
    printf("|%s|\n", s3);
  }
  free(s1);
  free(s2);
  free(s3);
  if (err) {
    fprintf(stderr, "Heap error\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
