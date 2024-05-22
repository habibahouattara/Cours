#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

void setNice(int inc);

int main(void) {
  setNice(10);
  setNice(-11);

  return EXIT_SUCCESS;
}

void setNice(int inc) {
  errno = 0;
  int n = nice(inc);
  if (n == -1 && errno != 0)  {
    perror("nice");
    exit(EXIT_FAILURE);
  }
  printf("Ma nouvelle valeur de politesse : %d\n", n);
}
