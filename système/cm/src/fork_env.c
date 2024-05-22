#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern char **environ;

/**
 * Exemple de lancement d'une commande en lui transmettant
 * les variables d'environnement.
 */

int main(void) {
  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    // Fonctionne aussi
    // execlp("./environ", "./environ", NULL);
    execle("./environ", "./environ", NULL, environ);
    perror("execlp");
    exit(EXIT_FAILURE);
  default:
    break;
  }

  return EXIT_SUCCESS;
}
