#include <stdlib.h>
#include <stdio.h>

void foo(void) {
  printf("$");
  foo();
}

int main(void) {
  foo();
  return EXIT_SUCCESS;
}
