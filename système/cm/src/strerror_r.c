#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ERR_MSG_SIZE 2
void my_perror(const char *s, int errnum);

void * run(void * arg);

int main(void) {
    int errnum;
    
    pthread_attr_t attr;
    if ((errnum = pthread_attr_init(&attr)) != 0) {
      fprintf(stderr, "pthread_attr_init: %s\n", strerror(errnum));
      exit(EXIT_FAILURE);
    }
    
    if ((errnum = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0) {
      fprintf(stderr, "pthread_attr_setdetachstate: %s\n", strerror(errnum));
      exit(EXIT_FAILURE);
    }
    
    pthread_t th;
    if ((errnum = pthread_create(&th, &attr, run, NULL)) != 0) {
      fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
      exit(EXIT_FAILURE);
    }
    
    pthread_exit(NULL);
}

void my_perror(const char *s, int errnum) {
  size_t msg_size = ERR_MSG_SIZE;
  char *msg = malloc(msg_size);
  if (msg == NULL) {
    perror("malloc()");
    exit(EXIT_FAILURE);
  }

  int r;
  do {
    r = strerror_r(errnum, msg, msg_size);
    if (r != 0) {
      if (errno == EINVAL) {
        perror("strerror_r()");
        exit(EXIT_FAILURE);
      } else {
        msg_size *= 2;
        msg = realloc(msg, msg_size);
        if (msg == NULL) {
          perror("realloc()");
          exit(EXIT_FAILURE);
        }
      }
    } else {
      fprintf(stderr, "%s: %s\n", s, msg);
    }
  } while (r != 0);
  
  free(msg);
}

void * run(void * arg) {
  if (arg != NULL) {
    fprintf(stderr, "Unexpected argument value\n");
    exit(EXIT_FAILURE);
  }
  
  pthread_t th = pthread_self();
  
  int errnum;
  // Autre méthode pour détacher un thread
  // Génère une erreur ici car le thread est déjà détaché
  if ((errnum = pthread_detach(th)) != 0) {
    my_perror("pthread_detach", errnum);
    exit(EXIT_FAILURE);
  }
  
  return NULL;
}

