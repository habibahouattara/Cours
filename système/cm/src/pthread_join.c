#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Exemple d'utilisation de pthread_join avec une valeur de retour
 */

void * run(void * arg);

int main(void) {
  int errnum;
  pthread_t th;

  if ((errnum = pthread_create(&th, NULL, run, NULL)) != 0) {
      fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
      exit(EXIT_FAILURE);
  }

  int *ptr;
  if ((errnum = pthread_join(th, (void **) &ptr)) != 0) {
      fprintf(stderr, "pthread_join: %s\n", strerror(errnum));
      exit(EXIT_FAILURE);
  }

  printf("Le thread secondaire vient de se terminer\n");

  if (ptr != PTHREAD_CANCELED && ptr != NULL) {
      printf("Valeur de retour : %i\n", *ptr);
      free(ptr);
  }

  return EXIT_SUCCESS;
}

void * run(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  // On crée une variable de type entière afin d'y stocker
  // une valeur de retour
  int *i = malloc(sizeof(int));
  if (i == NULL) {
    perror("malloc()");
    exit(EXIT_FAILURE);
  }
  *i = 1;
  
  return i;
}
