#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <pthread.h>

#define BUF_SIZE 255

void *run(void *arg);
void gestionnaire(int signum);
void gestion_signals();

int main(void) {
  int s = socket(AF_UNIX, SOCK_STREAM, 0);
  
  struct sockaddr_un ad = {AF_UNIX, "fic"};
  
  if (bind(s, (struct sockaddr*) &ad, sizeof(ad)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }
  
  listen(s, 3);
  
  gestion_signals();
  
  int service;
  pthread_t th;
  int errnum;
  
  while ((service = accept(s, NULL, NULL)) != -1) {
    int *ptr = malloc(sizeof(service));
    if (ptr == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    *ptr = service;
    errnum = pthread_create(&th, NULL, run, ptr);
  }
  
  return EXIT_SUCCESS;
}

void *run(void *arg) {
  int *socket = (int *)arg;
  ssize_t n;
  char buf[BUF_SIZE];
  
  while ((n = read(*socket, buf, sizeof(buf)) != -1)) {
    int v = atoi(buf);
    printf("server -> %d\n", v);
    if (v == 7) {
      write(*socket, "gagné", strlen("gagné"));
      break;
    } else {
      write(*socket, "perdu", strlen("perdu"));
    }
  }
  printf("un client a gagné fin du thread\n");
  return NULL;
}

void gestionnaire(int signum) {
  if (unlink("fic") == -1) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }
}

void gestion_signals() {
  sigset_t sigset;
  if (sigemptyset(&sigset) == -1) {
    perror("sigsetempty");
    exit(EXIT_FAILURE);
  }
  
  struct sigaction action;
  action.sa_handler = gestionnaire;
  action.sa_mask = sigset;
  action.sa_flags = 0;
  
  if (sigaction(SIGINT, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
  if (sigaction(SIGQUIT, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
  if (sigaction(SIGTERM, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  if (sigaction(SIGPIPE, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
  
  action.sa_handler = SIG_IGN;
  if (sigaction(SIGCHLD, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
}
