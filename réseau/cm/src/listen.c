/**
 * Un exemple d'utilisation de listen qui ne fait rien...
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

#include "adresse_serveur_unix.h"

#define BUF_SIZE 512

int main(int argc, char* argv[]) {
  // Création de la socket
  int s;
  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  // Affectation d'une adresse
  struct sockaddr_un addr;
  memset(&addr, '\0', sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, ADRESSE_SERVEUR, sizeof(addr.sun_path) - 1);

  if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }
  
  // Déclarer la socket comme une socket d'écoute
  if (listen(s, 10) == -1) {
    perror("listen");
    return EXIT_FAILURE;
  }

  // Ici devrait se trouver la partie gestion des connexions
  while (1) ;
  
  return EXIT_SUCCESS;
}
