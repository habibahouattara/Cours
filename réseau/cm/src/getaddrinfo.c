/**
 * Récupération de l'adresse IP d'une machine à partir
 * de son nom (getaddrinfo).
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define BUF_SIZE 256

/**
 * Récupère l'adresse de la structure d'adresse réseau à partir d'une adresse de socket.
 * La fonction gère les adresses IPv4 (champ sin_addr de l'adresse de socket) et IPv6
 * (champ sin6_addr de l'adresse de socket).
 * Elle retourne NULL si sa->sa_family est différent de AF_INET et AF_INET6
 */
void *get_in_addr(struct sockaddr *sa);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s hostname\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_CANONNAME;

  int s;
  struct addrinfo *result;
  s = getaddrinfo(argv[1], NULL, &hints, &result);

  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  if (result != NULL) {
    printf("Canonical name: %s\n", result->ai_canonname);
  }

  for (struct addrinfo* rp = result; rp != NULL; rp = rp->ai_next) {
    char buffer[BUF_SIZE];
    void *in_addr = get_in_addr(rp->ai_addr);
    if (inet_ntop(rp->ai_family, in_addr, buffer, sizeof(buffer)) == NULL) {
      perror("inet_ntop");
      exit(EXIT_FAILURE);
    }
    printf("Network address: %s\n", buffer);
  }

  freeaddrinfo(result);
  exit(EXIT_SUCCESS);
}

void *get_in_addr(struct sockaddr *sa) {
  switch (sa->sa_family) {
  case (AF_INET) :
    {
      struct sockaddr_in *sain = (struct sockaddr_in *) sa;
      return &sain->sin_addr;
    }
  case (AF_INET6) :
    {
      struct sockaddr_in6 *sain6 = (struct sockaddr_in6 *) sa;
      return &sain6->sin6_addr;
    }
  default:
    return NULL;
  }
}
