#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

// Définit l'adresse du serveur dans ADRESSE_SERVEUR
#include "adresse_serveur_unix.h"

// Adresse du client
#define ADRESSE_CLIENT "adresse_client.socket"

int main(int argc, char* argv[]) {
  int s;

  if ((s = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  // On affecte une adresse à notre socket
  struct sockaddr_un addr_client = {
    .sun_family = AF_UNIX,           // sun_family
    .sun_path   = ADRESSE_CLIENT     // sun_path
  };

  if (bind(s, (struct sockaddr*) &addr_client, sizeof(addr_client)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }

  // Envoi du message au serveur
  struct sockaddr_un addr = {
    .sun_family = AF_UNIX,           // sun_family
    .sun_path   = ADRESSE_SERVEUR    // sun_path
  };

  if (sendto(s, "bonjour", 8, 0, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
    perror("sendto");
    return EXIT_FAILURE;
  }

  if (close(s) == -1) {
    perror("close");
    return EXIT_FAILURE;
  }

  if (unlink(ADRESSE_CLIENT) == -1) {
    perror("unlink");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
