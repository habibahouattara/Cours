#include <stdio.h>
#include <string.h>
#include "sub2.h"

SET_IMPLEMENTATION(setstr, const char *, strcmp)

void setstr_print(const setstr *s) {
  setstr *s2 = setstr_duplicate(s);
  printf("{");
  while (!setstr_is_empty(s2)) {
    const char *x = setstr_min(s2);
    setstr_remove(s2, x);
    printf("%s", x);
    if (!setstr_is_empty(s2)) {
      printf(", ");
    }
  }
  printf("}");
  setstr_dispose(&s2);
}
