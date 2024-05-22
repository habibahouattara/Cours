#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>

/**
 * Exemple d'utilisation de condition
 */

/**
 * Un mutex à utiliser conjointement avec la condition
 */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * La condition qui va nous permettre de synchoniser les threads
 */
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/**
 * La condition à réaliser : pret == vrai
 */
int pret = 0;

/**
 * Une fonction qui attend que la variable "pret" soit à vrai avant
 * de continuer.
 * run() peut être vue comme un moniteur.
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
  
  // On crée les threads
  for (int i = 0; i < 2; ++i) {
    pthread_t  th;
    if ((errnum = pthread_create(&th, &attr, run, NULL)) != 0) {
      fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
      exit(EXIT_FAILURE);
    }
  }
  
  sleep(1);
    
  // On réalise la condition
  if ((errnum = pthread_mutex_lock(&mutex)) != 0) {
    fprintf(stderr, "pthread_mutex_lock: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);   
  }

  pret = 1;
  
  // On libère tous les threads en attente sur la condition
  // (une fois qu'on aura libéré le mutex)
  pthread_cond_broadcast(&cond);
  
  if ((errnum = pthread_mutex_unlock(&mutex)) != 0) {
    fprintf(stderr, "pthread_mutex_unlock: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);   
  }
  
  pthread_exit(NULL);
}

void * run(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  // On commence par prendre le mutex afin de vérifier la valeur de la
  // variable partagée (pret)
  int errnum;
  if ((errnum = pthread_mutex_lock(&mutex)) != 0) {
    fprintf(stderr, "pthread_mutex_lock: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);   
  }
  
  if (!pret) {
    // Il faut attendre
    // Le fait de se mettre en attente sur la condition libère le mutex
    printf("Thread en attente de la condition\n");
    pthread_cond_wait(&cond, &mutex);
  }
  
  printf("Valeur de pret : %d\n", pret);
  
  // On libère le mutex
  if ((errnum = pthread_mutex_unlock(&mutex)) != 0) {
    fprintf(stderr, "pthread_mutex_unlock: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);   
  }
  
  return NULL;
}
