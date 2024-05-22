/**
 * Convertit une adresse IPv4 en un nom d'hôte.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#ifndef NI_MAXHOST
// NI_MAXHOST n'est pas définie pas SUSv4 mais peut l'être sur
// certains systèmes.
#define NI_MAXHOST 1025
#endif

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s ip-addr\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // On commence par convertir la chaîne de caractère en une adresse
  // réseau manipulable par l'API.
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = 0; // Non utilisé ici
  int r;
  r = inet_pton(AF_INET, argv[1], &addr.sin_addr);
  switch (r) {
  case 0:
    fprintf(stderr, "L'adresse réseau transmise n'est pas une adresse IPv4\n");
    exit(EXIT_FAILURE);
  case -1:
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }
  
  // Reste plus qu'à interroger le DNS
  char host[NI_MAXHOST];
  int errnum;
  if ((errnum = getnameinfo((struct sockaddr *) &addr, sizeof(addr), host, sizeof(host), NULL, 0,  NI_NAMEREQD)) != 0) {
    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(errnum));
    exit(EXIT_FAILURE);
  }

  printf("Nom de machine pour l'adresse IP %s : %s\n", argv[1], host);

  return EXIT_SUCCESS;
}
