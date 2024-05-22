#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void *run1(void *i); 
void *run2(void *i); 

int main(void) {
  int i = 1;
  pthread_t t1;
  pthread_t t2;
  fprintf(stdout, "la valeur de i avant création de thread, %d\n", i); 
  if (pthread_create(&t1, NULL, run1, &i) != 0) {
    fprintf(stderr, "Erreur système");
    return EXIT_FAILURE;
  }
  if (pthread_create(&t2, NULL, run2, &i) != 0) {
    fprintf(stderr, "Erreur système");
    return EXIT_FAILURE;
  }
  sleep(1);
  pthread_exit(NULL);
}

void *run1(void * i) {
  int *p = (int *) i;
  *p += 2;
  printf("i dans thread 1 après incrémentation 2 fois, %d\n", *p);
  return NULL;
}

void *run2(void * i) {
  int *p = (int *) i;
  *p += 5;
  printf("i dans thread 2 après incrémentation 3 fois, %d\n", *p);
  return NULL;
}
