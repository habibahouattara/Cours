#include <stdlib.h>
#include <stdio.h>

void foo(void) {
  foo();
  printf("$");
}

int main(void) {
  foo();
  return EXIT_SUCCESS;
}
