#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>

/**
 * Exemple d'utilisation de mutex
 */

/**
 * Un mutex pour réaliser la synchronisation sur la variable
 * compteur (exclusion mutuelle).
 * Attention, par défaut, les mutex ne peuvent être utilisés
 * qu'à l'intérieur d'un même processus (même s'ils résident
 * dans un segment de mémoire partagée) : voir l'attribut
 * de mutex PTHREAD_PROCESS_SHARED si l'option
 * _POSIX_THREAD_PROCESS_SHARED est supportée par l'implémentation.
 */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Une fonction qui affiche un compteur puis l'incrémente.
 */
void * run(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  static int compteur = 0;
  
  int errnum;
  if ((errnum = pthread_mutex_lock(&mutex)) != 0) {
    fprintf(stderr, "pthread_mutex_lock: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);   
  }
  
  printf("Valeur du compteur : %d\n", compteur);
  ++compteur;
  
  if ((errnum = pthread_mutex_unlock(&mutex)) != 0) {
    fprintf(stderr, "pthread_mutex_unlock: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);   
  }
  
  
  return NULL;
}

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
