#include <stdlib.h>
#include <stdio.h>
#include "assume.h"

void assume__fail(const char *file, long int line, const char *func,
    const char *failedtest, const char *failure) {
  fprintf(stderr,
      "*** Failure: %s\n"
      "*** file: %s, line: %ld, function: %s\n"
      "*** failed test: %s\n",
      failure, file, line, func, failedtest);
  exit(EXIT_FAILURE);
}
