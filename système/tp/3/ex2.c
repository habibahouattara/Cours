#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "proc_carac.h"

void *run(void *i);

int main(void) {
  pid_t pid;
  pthread_t th;
  while (scanf("%d", &pid) == 1) {
    if (pthread_create(&th, NULL, run, &pid) != 0) {
      fprintf(stderr, "Erreur système");
      exit(EXIT_FAILURE);
    }
  }
  pthread_exit(NULL);
}

void *run(void *i) {
  if (i == NULL) {
    fprintf(stderr, "wrong argument\n");
    return NULL;
  }
  pid_t *pid = (pid_t *) i;
  char buf[30];
  
  sprintf(buf, "/proc/%d/status", pid);
  
  execlp("cat", "cat", buf, NULL);
  return NULL;
}
