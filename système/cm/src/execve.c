#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    printf("Mon pid : %d\n", getpid());
  const char* const argv[] = {"/bin/ps", "-j", NULL};
  const char* const envp[] = {NULL};

  // On exécute la commande ps -j
  execve(argv[0], (char* const *) argv, (char* const *) envp);

  perror("execve");
  return EXIT_FAILURE;
}
