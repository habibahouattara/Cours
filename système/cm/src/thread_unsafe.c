#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>

/**
 * Exemple d'utilisation d'une fonction non thread-safe.
 * On peut avoir un résultat comme :
...
Valeur du compteur : 87
Valeur du compteur : 88
Valeur du compteur : 89
Valeur du compteur : 89
Valeur du compteur : 91
Valeur du compteur : 92
...
 */

/**
 * Une fonction qui affiche un compteur puis l'incrémente.
 */
void * run(void * arg);

int main(void) {
    int errnum;
    
    pthread_attr_t attr;
    if ((errnum = pthread_attr_init(&attr)) != 0) {
      fprintf(stderr, "pthread_attr_init: %s\n", strerror(errnum));
      exit(EXIT_FAILURE);   
    }
    if ((errnum = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))!= 0) {
      fprintf(stderr, "pthread_attr_setdetachstate: %s\n", strerror(errnum));
      exit(EXIT_FAILURE);   
    } 
    
    for (int i = 0; i < 100; ++i) {
      pthread_t  th;
      if ((errnum = pthread_create(&th, &attr, run, NULL)) != 0) {
        fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
        exit(EXIT_FAILURE);
      }
    }
    
    pthread_exit(NULL);
    
}

void * run(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  static int compteur = 0;
  
  printf("Valeur du compteur : %d\n", compteur);
  ++compteur;
  
  return NULL;
}
