#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "str_scat.h"

char *str_scat(int separator, ...) {
  va_list ap;
  const char *p;
  size_t sep = ((char) separator != '\0');
  size_t n = (sep == 0);
  va_start(ap, separator);
  while ((p = va_arg(ap, const char *)) != NULL) {
    size_t k = strlen(p) + sep;
    if (n > SIZE_MAX - k) {
      return NULL;
    }
    n += k;
  }
  va_end(ap);
  char *s = malloc(n == 0 ? 1 : n);
  if (s == NULL) {
    return NULL;
  }
  char *q = s;
  va_start(ap, separator);
  while ((p = va_arg(ap, const char *)) != NULL) {
    while (*p != '\0') {
      *q = *p;
      ++q;
      ++p;
    }
    if (sep != 0) {
      *q = (char) separator;
      ++q;
    }
  }
  va_end(ap);
  if (sep != 0 && q > s) {
    --q;
  }
  *q = '\0';
  return s;
}
