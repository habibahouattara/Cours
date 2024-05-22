#include <unistd.h>    // getpid et getppid

#include <stdlib.h>    // EXIT_SUCCESS
#include <stdio.h>     // printf

int main(void) {
  printf("Mon identifiant de processus est : %d\n", getpid());
  printf("Mon père est : %d\n", getppid());

  return EXIT_SUCCESS;
}
