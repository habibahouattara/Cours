/**
 * Exemple de conversion d'une adresse en notation pointée en une adresse
 * manipulable par le réseau et vice versa.
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s ip-addr\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // On convertit l'adresse en notation pointée en une adresse réseau.
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
  
  // On réalise l'opération inverse
  char dotted_ip[16]; // Au max "xxx.xxx.xxx.xxx"
  if (inet_ntop(AF_INET, &addr.sin_addr, dotted_ip, sizeof(dotted_ip)) == NULL) {
    perror("inet_ntop");
    exit(EXIT_FAILURE);
  }

  printf("Adresse après translation puis opération inverse : %s\n", dotted_ip);

  return EXIT_SUCCESS;
}
