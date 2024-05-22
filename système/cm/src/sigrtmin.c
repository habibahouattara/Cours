#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

// Des exemples de déclaration de signaux
#define SIG_INIT       (SIGRTMIN)
#define SIG_ATTENTE    (SIGRTMIN + 1)
#define SIG_PRET       (SIGRTMIN + 2)
#define SIG_MAX_UNUSED (SIGRTMIN + 3)   

int main(void) {
  if (SIG_MAX_UNUSED >= SIGRTMAX) {
    fprintf(stderr, "Pas assez de signaux temps-réel disponibles\n");
    exit(EXIT_SUCCESS);
  }
  
  return EXIT_SUCCESS;
}
