#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

// Définit l'adresse du serveur dans ADRESSE_SERVEUR
#include "adresse_serveur_unix.h"

#define BUF_SIZE 512

int main(int argc, char* argv[]) {
  int s;

  if ((s = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  struct sockaddr_un addr = {
    .sun_family = AF_UNIX,           // sun_family
    .sun_path   = ADRESSE_SERVEUR    // sun_path
  };

  if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }

  char buf[BUF_SIZE];
  buf[BUF_SIZE - 1] = 0;
  struct sockaddr_un addr_from;
  socklen_t addr_len = sizeof(addr_from);
  if (recvfrom(s, buf, sizeof(buf) - 1, 0, (struct sockaddr*) &addr_from, &addr_len) == -1) {
    perror("recvfrom");
    return EXIT_FAILURE;
  }

  if (addr_len == 0) {
    strcpy(addr_from.sun_path, "unnamed");
  }
  printf("msgv received from %s: %s\n", addr_from.sun_path, buf);

  if (close(s) == -1) {
    perror("close");
    return EXIT_FAILURE;
  }
  if (unlink(ADRESSE_SERVEUR) == -1) {
    perror("unlink");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
