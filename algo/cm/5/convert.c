#include <stdlib.h>
#include <stdio.h>

#define CONVERT(from_type, from, to_type, to, expr_in_terms_of_from)           \
  to_type from ## _to_ ## to(from_type from) {                                 \
    return expr_in_terms_of_from;                                              \
  }

CONVERT(double, farenheit, double, celsius, (farenheit - 32.0) * 5.0 / 9.0)
CONVERT(double, celsius, double, farenheit, celsius * 9.0 / 5.0 + 32.0)

#define CHECK_FC(x) \
  printf("%lf °F -> %lf °C\n", (x), farenheit_to_celsius(x))
#define CHECK_CF(x) \
  printf("%lf °C -> %lf °F\n", (x), celsius_to_farenheit(x))

int main(void) {
  CHECK_CF(0.0);
  CHECK_CF(100.0);
  CHECK_FC(32.0);
  CHECK_FC(212.0);
  return EXIT_SUCCESS;
}
