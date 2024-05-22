#include "socket_tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void build_inet_address(struct sockaddr* addr, socklen_t* addr_size, const char* host, const char* port) {
  struct addrinfo hints = {
      // On veut des adresses pour les sockets SOCK_STREAM
      .ai_socktype = SOCK_STREAM,
      // Si host est null alors on veut une adresse IP joker,
      // on ne veut que des adresses utilisables par au moins une interface réseau du système
      .ai_flags = AI_PASSIVE | AI_ADDRCONFIG      
  };

  struct addrinfo *result;
  int s = getaddrinfo(host, port, &hints, &result);

  if (s != 0) {
    fprintf(stderr, "build_inet_address(): getaddrinfo(): %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  if (result->ai_addr->sa_family != AF_INET && result->ai_addr->sa_family != AF_INET6) {
    fprintf(stderr, "build_inet_address(): invalid address\n");
    exit(EXIT_FAILURE);    
  }

  if (*addr_size <  result->ai_addrlen) {
      fprintf(stderr, "build_inet_address(): not enough space to store address\n");
      exit(EXIT_FAILURE);    
  }
  *addr_size = result->ai_addrlen;
  memcpy(addr, result->ai_addr, *addr_size);

  freeaddrinfo(result);
}

void display_socket_address(int s) {
  struct sockaddr_storage addr;
  socklen_t length;
  length = sizeof addr;
  if (getsockname(s, (struct sockaddr*) &addr, &length) == -1) {
    perror("getsockname()");
    exit(EXIT_FAILURE);
  }

  display_inet_address((struct sockaddr*) &addr, length);
}

void display_inet_address(struct sockaddr* addr, socklen_t addr_size) {
  char host[NI_MAXHOST];
  char serv[NI_MAXSERV];
  int s;
  if ((s = getnameinfo((struct sockaddr*) addr, addr_size, host, NI_MAXHOST, serv, NI_MAXSERV, 0)) != 0) {
    fprintf(stderr, "display_inet_address(): getnameinfo(): %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  printf("Host = %s, Port = %s\n", host, serv);  
}
