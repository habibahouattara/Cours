#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

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

  // On associe l'action aux signaux
  for (int i = 1; i < 32; ++i) {
    if (i != SIGKILL && i != SIGSTOP && sigaction(i , &action, NULL) == -1) {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  }

  // Un read qui peut etre interrompu par un signal
  ssize_t n;
  char buffer[1024];
  while ((n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0) {
    buffer[n] = 0;
    printf("Message lu : %s\n", buffer);
  }

  if (n == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}

void gestionnaire(int signum) {
  printf("Signal numéro %d reçu\n", signum);
}
