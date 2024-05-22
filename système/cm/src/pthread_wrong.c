#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Un MAUVAIS exemple d'utilisation de thread avec traitement de la valeur de retour (une adresse sur la pile du thread qui vient d'être libéré...)
 * L'exemple est suffisamment alambiqué pour que le compilateur (ou tout au moins gcc) ne détecte pas l'erreur
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
  }

  return EXIT_SUCCESS;
}

void * run(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  int t[2] = { 0, 1 };
  int *p = t + 1;
  
  return p;
}
