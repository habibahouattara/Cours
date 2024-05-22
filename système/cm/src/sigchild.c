#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

/**
 * Un chasseur de zombis.
 */
void gestionnaire_SIGCHLD(int signum);

int main(void) {
  struct sigaction action;
  action.sa_handler = gestionnaire_SIGCHLD;
  action.sa_flags = 0;
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

void gestionnaire_SIGCHLD(int signum) {
  if (signum != SIGCHLD) {
    fprintf(stderr, "Mauvais signal reçu.\n");
    exit(EXIT_FAILURE);
  }

  // On sauvegarde errno pour pouvoir la restaurer à la fin du gestionnaire.
  int errno_old = errno;

  // Comme plusieurs fils ont pu mourir avant que l'ordonnanceur
  // ne nous redonne la main, plusieurs signaux SIGCHLD ont pu
  // nous être adressés.
  // Comme lorsque le processus est élu par l'ordonnanceur un seul
  // signal SIGCHLD sera finalement traité, il faut faire une boucle
  // pour retirer tous les fils zombis.
  int r; // Valeur de retour du wait
  do {
    // Comme on ne sait combien il y a de zombis, on
    // fait des wait non bloquants jusqu'à ce qu'il n'y ai plus de zombis.
    r = waitpid(-1, NULL, WNOHANG);
  } while (r > 0);

  // Si r = 0, il y a des fils toujours vivant mais plus de zombis
  // Si r = -1, soit il y a une erreur, soit il n'y a plus de zombis
  if (r == -1) {
    // Si wait a retourné parcequ'il n'y a plus de zombis, alors
    // errno vaut ECHILD
    if (errno != ECHILD) {
      perror("waitpid");
      exit(EXIT_FAILURE);
    }
  }

  errno = errno_old;
}
