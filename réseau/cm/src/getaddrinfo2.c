/**
 * Récupération d'un numéro de port à partir du nom du service (getaddrinfo).
 */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h> // memset

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s service-name\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET; // Pour être sûr d'avoir des struct sockaddr_in en résultat

  int s;
  struct addrinfo *result;
  s = getaddrinfo(NULL, argv[1], &hints, &result);

  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  if (result != NULL) {
    for (struct addrinfo* rp = result; rp != NULL; rp = rp->ai_next) {
      printf("Port : %d\n", ntohs(((struct sockaddr_in*) (rp->ai_addr))->sin_port));
    }
  } else {
    fprintf(stderr, "Pas de service trouvé\n");
  }

  freeaddrinfo(result);
  exit(EXIT_SUCCESS);
}
