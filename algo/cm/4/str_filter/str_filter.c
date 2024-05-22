#include <stdlib.h>
#include "str_filter.h"

char *str_filter(const char *s, int (*filter)(int)) {
  size_t n = 0;
  for (const char *p = s; *p != '\0'; ++p) {
    if (filter(*p)) {
      ++n;
    }
  }
  char *s2 = malloc(n + 1);
  if (s2 == NULL) {
    return NULL;
  }
  char *p2 = s2;
  for (const char *p = s; *p != '\0'; ++p) {
    if (filter(*p)) {
      *p2 = *p;
      ++p2;
    }
  }
  *p2 = '\0';
  return s2;
}
