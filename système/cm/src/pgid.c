#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
  pid_t pid = getpid();

  printf("Identifiants du processus et de son groupe : %d %d\n",
         pid, getpgrp());

  // On fixe le groupe du processus comme étant le même que celui du père.
  if (setpgid(pid, getppid()) == -1) {
    perror("setpgid");
    exit(EXIT_FAILURE);
  }

  printf("Identifiants du processus et de son groupe : %d %d\n",
         pid, getpgrp());

  return EXIT_SUCCESS;
}
