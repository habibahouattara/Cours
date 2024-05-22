#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char* argv[]) {
  // Création de la socket
  int s;
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  // Affectation d'une adresse
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(50000);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }

  // Déclarer la socket comme une socket d'écoute
  if (listen(s, 10) == -1) {
    perror("listen");
    return EXIT_FAILURE;
  }

  // Boucle d'attente de connexions
  while (1) ;

  return EXIT_SUCCESS;
}
