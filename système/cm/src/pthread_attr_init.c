#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Exemple de création d'un thread utilisant les valeurs d'attributs par défaut
 * tout en utilisant une variable de type pthread_attr_t
 */

void * run(void * arg);

int main(void) {
  int errnum;
  
  pthread_attr_t attr;
  if ((errnum = pthread_attr_init(&attr)) != 0) {
    fprintf(stderr, "pthread_attr_init: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);
  }
  
  pthread_t th;
  if ((errnum = pthread_create(&th, &attr, run, NULL)) != 0) {
    fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);
  }
  
  if ((errnum = pthread_attr_destroy(&attr)) != 0) {
    fprintf(stderr, "pthread_attr_destroy: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);
  }
  
  pthread_exit(NULL);
}

void * run(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Bonjour\n");
  
  return NULL;
}
