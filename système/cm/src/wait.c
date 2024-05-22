#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>

void waitChild(void);

int main(void) {
  pid_t p;

  p = fork();
  switch(p) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    sleep(3);
    exit(15);
  default:
    waitChild();
    exit(EXIT_SUCCESS);
  }
}

void waitChild(void) {
    int status;
    if (wait(&status) == -1) {
      perror("wait");
      exit(EXIT_FAILURE);
    }
    printf("Un de mes fils est mort\n");
    if (WIFEXITED(status)) {
      printf("Il est mort normalement en retournant %d\n", WEXITSTATUS(status));
    }
}
