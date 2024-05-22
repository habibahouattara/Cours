#include <stdlib.h>
#include <stdio.h>

int main(void) {
  int n;
  if (scanf("%d", &n) != 1) {
    return EXIT_FAILURE;
  }
  int h[n];
  h[0] = 1;
  h[1] = 1;
  for (int k = 2; k < n; ++k) {
    h[k] = 1 + h[(k - 1) / 2];
    if (h[k] == h[k - 1] + 1) {
      printf("--------------------\n");
    }
    printf("%3d :%d\n", k, h[k]);
  }
  return EXIT_SUCCESS;
}
