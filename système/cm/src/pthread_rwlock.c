#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>

/**
 * Exemple d'utilisation de verrou de lecture/écriturecondition
 */

static int data = 0;

/**
 * Le verrou synchronisant les lectures/écritures sur la donnée
 */
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

/**
 * Une fonction qui lit la variable partagée.
 */
void * thread_read(void * arg);

/**
 * Une fonction qui incrémente la variable partagée.
 */
void * thread_write(void * arg);

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
    for (int i = 0; i < 100; ++i) {
      pthread_t  th;
      if ((errnum = pthread_create(&th, &attr, thread_read, NULL)) != 0) {
        fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
        exit(EXIT_FAILURE);
      }
      if ((errnum = pthread_create(&th, &attr, thread_write, NULL)) != 0) {
        fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
        exit(EXIT_FAILURE);
      }
    }

    pthread_exit(NULL);
}

void * thread_read(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  int errnum;
  
  if ((errnum = pthread_rwlock_rdlock(&rwlock)) != 0) {
    fprintf(stderr, "pthread_rwlock_rdlock: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);   
  }
  
  printf("<r>");
  if (fflush(stdout) != 0) {
    perror("fflush");
    exit(EXIT_FAILURE);
  }
  
  // On récupère la valeur de la donnée
  int t = data;
  printf("%d", t);
  if (fflush(stdout) != 0) {
    perror("fflush");
    exit(EXIT_FAILURE);
  }
  
  printf("</r>");
  if (fflush(stdout) != 0) {
    perror("fflush");
    exit(EXIT_FAILURE);
  }
  
  if ((errnum = pthread_rwlock_unlock(&rwlock)) != 0) {
    fprintf(stderr, "pthread_rwlock_unlock: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);   
  } 
  
  return NULL;
}

void * thread_write(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  int errnum;
  
  if ((errnum = pthread_rwlock_wrlock(&rwlock)) != 0) {
    fprintf(stderr, "pthread_rwlock_wrlock: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);   
  }
  
  printf("<w>");
  if (fflush(stdout) != 0) {
    perror("fflush");
    exit(EXIT_FAILURE);
  }
  
  // On modifie la valeur de la donnée
  int t = ++data;
  printf("%d", t);
  if (fflush(stdout) != 0) {
    perror("fflush");
    exit(EXIT_FAILURE);
  }
  
  printf("</w>");
  if (fflush(stdout) != 0) {
    perror("fflush");
    exit(EXIT_FAILURE);
  }
  
  if ((errnum = pthread_rwlock_unlock(&rwlock)) != 0) {
    fprintf(stderr, "pthread_rwlock_unlock: %s\n", strerror(errnum));
    exit(EXIT_FAILURE);   
  } 
  
  return NULL;
}
