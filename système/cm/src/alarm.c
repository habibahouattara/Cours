#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

/**
 * Exemple de alarm : on endort le processus en bloquant tous
 * les signaux à part SIGALRM et on programme un réveil dans 2 secondes.
 */

void gestionnaire(int signum);

int main(void) {
  sigset_t set;
  if (sigfillset(&set) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }

  // On commence par bloquer tous les signaux afin de ne pas
  // etre dérangé pendant notre initialisation
  if (sigprocmask(SIG_SETMASK, &set, NULL) == -1) {
    perror("sigprocmask");
    exit(EXIT_FAILURE);
  }

  // On installe le gestionnaire pour SIGALRM
  struct sigaction act;
  act.sa_handler = gestionnaire;
  if (sigfillset(&act.sa_mask) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }
  act.sa_flags = 0;
  if (sigaction(SIGALRM, &act, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  // L'appel réussit toujours.
  alarm(2);

  printf("Je me couche et j'attends le réveil\n");
  if (sigdelset(&set, SIGALRM) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }
  sigsuspend(&set);
  if (errno != EINTR) {
    perror("sigsuspend");
    exit(EXIT_FAILURE);
  }

  printf("Fin du processus.\n");
  return EXIT_SUCCESS;
}

void gestionnaire(int signum) {
  if (signum < 0) {
    fprintf(stderr, "Invalid signal  number\n");
    exit(EXIT_FAILURE);
  }
  printf("Je me réveille.\n");
}
