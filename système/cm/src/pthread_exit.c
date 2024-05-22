#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void * run(void * arg);

int main(void) {
  pthread_t th;
  if (pthread_create(&th, NULL, run, NULL) != 0) {
      fprintf(stderr, "Erreur\n");
      exit(EXIT_FAILURE);
  }
  //sleep(20);
  pthread_exit(NULL);
}

void * run(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Bonjour\n");
  sleep(20);
  return NULL;
}
