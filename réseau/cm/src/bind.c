/**
 * Création d'une socket datagram unix puis
 * affectation d'une adresse à cette socket.
 */

#include <sys/socket.h>
#include <sys/un.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Définit l'adresse du serveur dans ADRESSE_SERVEUR
#include "adresse_serveur_unix.h"

int main() {
  int socket_fd;

  if ((socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_un adresse_unix = {
    .sun_family = AF_UNIX,           // sun_family
    .sun_path   = ADRESSE_SERVEUR    // sun_path
  };

  if (bind(socket_fd, (struct sockaddr*) &adresse_unix, sizeof(adresse_unix)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  // Reste plus qu'à utiliser la socket ....
  while (1) ;

  return EXIT_SUCCESS;
}
