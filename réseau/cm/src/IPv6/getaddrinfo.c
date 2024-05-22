#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

#ifndef NI__MAXSERV
#define NI_MAXSERV 32
#endif

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s hostname\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  //hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_CANONNAME;

  // Ou plus "moderne" :
  // struct addrinfo hints = {.ai_family = AF_INET6, .ai_socktype = SOCK_STREAM, .ai_flags = AI_CANONNAME};

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
    char host[NI_MAXHOST]; // NI_MAXHOST définit avec _BSD_SOURCE

    int s;
    if ((s = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST,
                         NULL, 0, NI_NUMERICHOST)) != 0) {
      fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
      exit(EXIT_FAILURE);
    }

    printf("IP address: %s\n", host);
  }

  freeaddrinfo(result);
  exit(EXIT_SUCCESS);
}
