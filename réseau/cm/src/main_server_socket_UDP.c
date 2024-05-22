/**
 * Un mini serveur UDP qui se contente d'afficher le premier message qu'il reçoit.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 512

#define HOST_SIZE_STRING 256
#define SERVICE_SIZE_STRING 256

int main(int argc, char* argv[]) {
  int s;

  if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  // On construit l'adresse de notre serveur.
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
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

  // On affecte l'adresse à notre serveur
  if (bind(s, (struct sockaddr*) res->ai_addr, sizeof(struct sockaddr)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }
  freeaddrinfo(res);

  char buf[BUF_SIZE];
  buf[BUF_SIZE - 1] = 0;
  struct sockaddr_in addr_from;
  socklen_t addr_len = sizeof(addr_from);
  if (recvfrom(s, buf, sizeof(buf) - 1, 0, (struct sockaddr*) &addr_from, &addr_len) == -1) {
    perror("recvfrom");
    return EXIT_FAILURE;
  }

  if (addr_len == 0) {
    printf("Adresse client inconnue");
  } else {
    char host[HOST_SIZE_STRING];
    char service[SERVICE_SIZE_STRING];
    if ((r = getnameinfo((struct sockaddr *) &addr_from, addr_len,
                         host, HOST_SIZE_STRING,
                         service, SERVICE_SIZE_STRING, 0)) != 0) {
      fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(s));
      exit(EXIT_FAILURE);
    }
    printf("Message reçu de %s:%s\n", host, service);
  }
  printf("Message : %s\n", buf);


  if (close(s) == -1) {
    perror("close");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
