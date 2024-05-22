#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Exemple de création d'un thread "ordinaire".
 */

void * run(void * arg);

int main(void) {
  int errnum;
  pthread_t th;

  if ((errnum = pthread_create(&th, NULL, run, NULL)) != 0) {
      fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
      exit(EXIT_FAILURE);
  }

  // Suite du thread principal

  return EXIT_SUCCESS;
}

void * run(void * arg) {
  // Exécution du thread secondaire ...
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  return NULL;
}
