#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void gestionnaire(int signum);

int main(void) {
  // On construit notre action
  struct sigaction action;
  action.sa_handler = gestionnaire;
  action.sa_flags = 0;
  if (sigfillset(&action.sa_mask) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }

  // On associe l'action à SIGINT
  if (sigaction(SIGINT, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  // On attend le signal
  while (1) ;

  return EXIT_SUCCESS;
}

void gestionnaire(int signum) {
  if (signum < 0) {
    fprintf(stderr, "Wrong signal number\n");
    exit(EXIT_FAILURE);
  }
  printf("Au revoir\n");
  exit(EXIT_SUCCESS);
}
