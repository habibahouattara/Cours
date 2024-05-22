#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

/**
 * Exemple de sigsupend : on endort le processus en bloquant tous
 * les signaux à part SIGINT.
 * Lorsque SIGINT est reçu le processus se réveille, affiche un message
 * via son gestionnaire et se termine.
 */

void gestionnaire(int signum);

int main(void) {
  sigset_t set;
  if (sigfillset(&set) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }

  // On bloque tous les signaux
  // Question : pourquoi ?
  if (sigprocmask(SIG_SETMASK, &set, NULL) == -1) {
    perror("sigprocmask");
    exit(EXIT_FAILURE);
  }

  struct sigaction act;
  act.sa_handler = gestionnaire;
  if (sigfillset(&act.sa_mask) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }
  act.sa_flags = 0;
  if (sigaction(SIGINT, &act, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  // On construit un ensemble contenant tous les signaux
  // sauf SIGINT
  if (sigdelset(&set, SIGINT) == -1) {
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
  printf("Au revoir.\n");
}
