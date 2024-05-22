#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#include "socket_tools.h"

#define BUF_SIZE 512

void new_thread(int socket);
void* process_connexion(void* socketp);
void perror_r(int errno, const char* s);

int main(void) {
  // Construction de l'adresse réseau
  struct sockaddr_storage addr;
  socklen_t addr_size = sizeof(addr);
  build_inet_address((struct sockaddr*) &addr, &addr_size, "::1", "44400");

  // Création de la socket
  int s;
  if ((s = socket(addr.ss_family, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  // Affectation d'une adresse
  if (bind(s, (struct sockaddr*) &addr, addr_size) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }
  display_socket_address(s);

  // Déclarer la socket comme une socket d'écoute
  if (listen(s, 10) == -1) {
    perror("listen");
    return EXIT_FAILURE;
  }

  // Boucle d'attente de connexions
  int service;
  while ((service = accept(s, (struct sockaddr*) &addr, &addr_size)) != -1) {
    display_inet_address((struct sockaddr*) &addr, addr_size);
    new_thread(service);
  }

  perror("accept");
  return EXIT_FAILURE;
}

void new_thread(int socket) {
  int ret;

  pthread_attr_t attr;
  if ((ret = pthread_attr_init(&attr)) != 0) {
    perror_r(ret, "pthread_attr_init()");
    exit(EXIT_FAILURE);
  }
  
  // On détache le thread afin de ne pas avoir à faire de join
  if ((ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0) {
    perror_r(ret, "pthread_attr_setdetachstate()");
    exit(EXIT_FAILURE);
  }

  int* socketp = malloc(sizeof (int));
  *socketp = socket;
  pthread_t t;
  if ((ret = pthread_create(&t, &attr, process_connexion, (void*) socketp)) != 0) {
    perror_r(ret, "pthead_create()");
    exit(EXIT_FAILURE);
  }

  if ((ret = pthread_attr_destroy(&attr)) != 0) {
    perror_r(ret, "pthread_attr_destroy()");
    exit(EXIT_FAILURE);
  }
}

void* process_connexion(void* socketp) {
  int socket = *(int *) socketp;
  char buf[BUF_SIZE];
  buf[BUF_SIZE - 1] = 0;

  ssize_t n;
  while ((n = read(socket, buf, BUF_SIZE - 1)) > 0) {
    printf("message received (%d bytes): %s\n", (int) n, buf);
  }
  
  if (n == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  
  if (close(socket) == -1) {
    perror("close");
  }

  free(socketp);
  pthread_exit(NULL);
}

void perror_r(int errno, const char* s) {
  fprintf(stderr, "%s: %s\n", s, strerror(errno));
}
