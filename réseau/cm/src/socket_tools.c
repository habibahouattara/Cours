#include "socket_tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

void build_inet_address(struct sockaddr_in* addr, const char* host, const char* port) {
  memset(addr, 0, sizeof(struct sockaddr_in));
  addr->sin_family = AF_INET;
  //  if (inet_aton(host, &(addr->sin_addr)) == 0) {
  switch (inet_pton(AF_INET, host, &(addr->sin_addr))) {
  case 0:
    {
    // Ce n'est pas une adresse IP, on essaie de résoudre l'adresse DNS
    struct hostent* hostent;
    if ((hostent = gethostbyname(host)) == NULL) {
      fprintf(stderr, "build_address: invalid address\n");
      exit(EXIT_FAILURE);
    }
    //addr->sin_addr.s_addr = ((struct in_addr*)(hostent->h_addr_list[0]))->s_addr;
    addr->sin_addr = *((struct in_addr*)(hostent->h_addr_list[0]));
    }
    break;
  case -1:
      perror("inet_pton");
      exit(EXIT_FAILURE);
  default:
    // La convertion a réussi
    break;
  }

  int numero;
  if (sscanf(port, "%d", &numero) == 1) {
    addr->sin_port = htons(numero);
  } else {
    // Le port n'est pas un numéro, on essaie de résoudre le nom du service
    struct servent* servent;
    if ((servent = getservbyname(port, "tcp")) == NULL) {
      fprintf(stderr, "build_address: unknown port\n");
      exit(EXIT_FAILURE);
    }
    addr->sin_port = servent->s_port;
  }
}

void display_inet_address(int s) {
  struct sockaddr_in addr;
  socklen_t length;
  length = sizeof(struct sockaddr_in);
  if (getsockname(s, (struct sockaddr*)&addr, &length) == -1) {
    perror("getsockname");
    exit(EXIT_FAILURE);
  }
  printf("IP = %s, Port = %u\n",
	 inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
}
