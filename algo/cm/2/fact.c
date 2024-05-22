#include <stdlib.h>
#include <stdio.h>

//  fact : renvoie la factorielle de son argument sous réserve que cette
//    factorielle soit exprimable sur le type unsigned int
unsigned int fact(unsigned int n);

int main(void) {
  for (unsigned int k = 0; k < 36; ++k) {
    printf("%u %u\n", k, fact(k));
  }
  return EXIT_SUCCESS;
}

unsigned int fact(unsigned int n) {
  if (n == 0) {
    return 1;
  } else {
    return n * fact(n - 1);
  }
}
