/**
 * Un processus qui tente d'écrire dans un tube qui n'a plus de lecteurs.
 * Il se termine lors de l'écriture car il reçoit le signal SIGPIPE.
 */

// Pour SIGPIPE
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(void) {
  int tube[2];
  const char buffer[] = "Bonjour\n";
  int status;

  // On crée un fils afin de pouvoir vérifier qu'il se termine
  // à cause d'un signal
  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    if (pipe(tube) == -1) {
      perror("pipe");
      exit(EXIT_FAILURE);
    }
    
    if (close(tube[0]) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
    
    if (write(tube[1], buffer, sizeof(buffer)) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }

    printf("Une partie du code qui n'est pas exécutée...\n");
    exit(EXIT_SUCCESS);
  default:
    if (wait(&status) == -1) {
      perror("wait");
      exit(EXIT_FAILURE);
    }
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGPIPE) {
      printf("Mon fils est mort à cause de SIGPIPE\n");
    }
    exit(EXIT_SUCCESS);
  }
}
