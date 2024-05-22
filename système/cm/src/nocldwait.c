/**
 * Utilisation de SA_NOCLDWAIT afin que les fils
 * ne soient pas transformés en zombis à leur mort.
 * Il s'agit du vaccin contre les zombis.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void) {
  struct sigaction action;
  action.sa_handler = SIG_DFL;
  action.sa_flags = SA_NOCLDWAIT;
  if (sigfillset(&action.sa_mask) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }
  if (sigaction(SIGCHLD, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  // On crée 10 zombis
  for (int i = 0; i < 10; ++i) {
    switch (fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      // Le fils : on en fait un zombi
      exit(EXIT_SUCCESS);
    default:
      break;
    }
  }

  // On terminera par un signal : cela nous laisse le temps de
  // voir ce qu'il se passe...
  while (1) ;

  return EXIT_SUCCESS;

}
