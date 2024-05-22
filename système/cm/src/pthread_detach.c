#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Exemple d'utilisation de pthread_detach
 */

void * run(void * arg);

int main(void) {
  int errnum;
  
  pthread_attr_t attr;
  if ((errnum = pthread_attr_init(&attr)) != 0) {
    fprintf(stderr, "pthread_attr_init: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);
  }

  if ((errnum = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0) {
    fprintf(stderr, "pthread_attr_setdetachstate: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);
  }
  
  pthread_t th;
  if ((errnum = pthread_create(&th, &attr, run, NULL)) != 0) {
    fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);
  }
    
  pthread_exit(NULL);
}

void * run(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  pthread_t th = pthread_self();
  
  int errnum;
  // Autre méthode pour détacher un thread
  // Génère une erreur ici car le thread est déjà détaché
  if ((errnum = pthread_detach(th)) != 0) {
    fprintf(stderr, "pthread_detach: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);
  }
  
  return NULL;
}

