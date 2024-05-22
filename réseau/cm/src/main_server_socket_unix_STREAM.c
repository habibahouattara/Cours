#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

#define BUF_SIZE 512

#include "adresse_serveur_unix.h"

void process_connexion(int socket);
void quit(int signum);
void connect_signals();

int main(int argc, char* argv[]) {
  // Création de la socket
  int s;
  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  // Affectation d'une adresse
  struct sockaddr_un addr = {
    .sun_family = AF_UNIX,           // sun_family
    .sun_path   = ADRESSE_SERVEUR    // sun_path
  };

  if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }
  // Déclarer la socket comme une socket d'écoute
  if (listen(s, 10) == -1) {
    perror("listen");
    return EXIT_FAILURE;
  }

  connect_signals();
  
  // Boucle d'attente de connexions
  int service;
  while ((service = accept(s, NULL, NULL)) != -1) {
    process_connexion(service);
  }

  perror("accept");
  return EXIT_FAILURE;
}

void process_connexion(int socket) {
  char buf[BUF_SIZE];
  buf[BUF_SIZE - 1] = 0;

  if (read(socket, buf, BUF_SIZE - 1) == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  printf("message received: %s\n", buf);
  close(socket);
}

void quit(int signum) {
 if (unlink(ADRESSE_SERVEUR) == -1) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }
 exit(EXIT_SUCCESS);
}

void connect_signals() {
  sigset_t sigset;
  if (sigemptyset(&sigset) == -1) {
    perror("sigemptyset");
    exit(EXIT_FAILURE);
  }

  struct sigaction action;
  action.sa_handler = quit;
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

  action.sa_handler = SIG_IGN;
  if (sigaction(SIGCHLD, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
}

