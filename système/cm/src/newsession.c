/**
 * Procédure classique pour créer une nouvelle session.
 */

// Pour getsid
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
  /**
   * Un leader de groupe de processus ne peut créer une nouvelle session,
   * on crée donc un fils pour lequel on est sur qu'il n'est pas leader de groupe.
   */
  printf("Je suis le processus %d et l'identifiant de ma session est %d\n", getpid(), getsid(0));

  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    if (setsid() == -1) {
      perror("setsid");
      exit(EXIT_FAILURE);
    }
    printf("Nouvelle session %d crée par le processus %d\n", getsid(0), getpid());
    exit(EXIT_SUCCESS);
  default:
     exit(EXIT_SUCCESS);
  }

  return EXIT_SUCCESS;
}
