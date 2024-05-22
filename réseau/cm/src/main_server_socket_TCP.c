#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

#include "socket_tools.h"

#define BUF_SIZE 512

void process_connexion(int socket) {
  char buf[BUF_SIZE + 1];
  // On met un zéro en fin de tampon pour que si on reçoit une chaîne
  // trop grande, elle soit tronquée et terminée par '\0'
  buf[BUF_SIZE] = 0;

  ssize_t n;
  while ((n = read(socket, buf, BUF_SIZE)) > 0) {
    write(STDOUT_FILENO, "received: ", 10);
    write(STDOUT_FILENO, buf, n);
  }
  
  if (n == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  
  printf("socket closed\n");

  close(socket);
}

int main(int argc, char* argv[]) {
  // Création de la socket
  int s;
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  // Affectation d'une adresse
  struct sockaddr_in addr;
  build_inet_address(&addr, "localhost", "44400");
  // Ou plus simple pour l'adresse IP
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // On autorise l'adresse locale à être réutilisée si on ne l'utilise plus
  int reuse = 1;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
    perror("setsockopt");
    return EXIT_FAILURE;
  }

  if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }
  display_inet_address(s);

  // Déclarer la socket comme une socket d'écoute
  if (listen(s, 10) == -1) {
    perror("listen");
    return EXIT_FAILURE;
  }

  // Boucle d'attente de connexions
  int service;
  while ((service = accept(s, NULL, NULL)) != -1) {
      process_connexion(service);
  }

  perror("accept");
  return EXIT_FAILURE;
}
