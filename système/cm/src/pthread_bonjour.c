#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void * run(void * arg);

int main(void) {
  pthread_t th;
  if (pthread_create(&th, NULL, run, NULL) != 0) {
      fprintf(stderr, "Erreur\n");
      exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}

void * run(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  printf("Bonjour\n");
  return NULL;
}
