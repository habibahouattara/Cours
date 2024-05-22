/**
 * Un client UDP qui se contente d'envoyer un bonjour par datagramme.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char* argv[]) {
  int s;

  if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  // On construit l'adresse du serveur.
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  struct addrinfo *res;
  int r;
  if ((r = getaddrinfo(NULL, "50000", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }
  if (res == NULL) {
    fprintf(stderr, "Erreur lors de la construction de l'adresse\n");
    exit(EXIT_FAILURE);
  }

  if (sendto(s, "bonjour", 8, 0, (struct sockaddr*) res->ai_addr, sizeof(struct sockaddr)) == -1) {
    perror("sendto");
    return EXIT_FAILURE;
  }

  freeaddrinfo(res);

  if (close(s) == -1) {
    perror("close");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
