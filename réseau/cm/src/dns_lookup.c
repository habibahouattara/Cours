// herror
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s hostname\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct hostent* hostent_p = gethostbyname(argv[1]);
  if (hostent_p == NULL) {
    herror("gethostbyname");
    exit(EXIT_FAILURE);
  }

  if (hostent_p->h_addrtype != AF_INET) {
    fprintf(stderr, "invalid address family\n");
    exit(EXIT_FAILURE);    
  }

  struct in_addr* in =
    (struct in_addr*) hostent_p->h_addr_list[0];
  printf("IP address: %s\n", inet_ntoa(*in));

  exit(EXIT_SUCCESS);
}
