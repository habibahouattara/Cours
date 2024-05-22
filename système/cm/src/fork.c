#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    pid_t p = fork();
    switch (p) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      printf("Je suis le processus fils, mon pid est %d et mon groupe de processus est %d\n", getpid(), getpgrp());
      exit(EXIT_SUCCESS);
    default:
      printf("Je suis le processus père, mon pid est %d et le pid de mon fils est %d\n", getpid(), p);
      exit(EXIT_SUCCESS);
    }
}
