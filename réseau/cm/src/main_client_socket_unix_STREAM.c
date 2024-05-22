#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "adresse_serveur_unix.h"

int main(int argc, char* argv[]) {
  int s;

  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  struct sockaddr_un addr = {
    .sun_family = AF_UNIX,           // sun_family
    .sun_path   = ADRESSE_SERVEUR    // sun_path
  };

  if (connect(s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
    perror("connect");
    return EXIT_FAILURE;
  }

  sleep(500);

  char buf[] = "bonjour";
  if (write(s, buf, 8) == -1) {
      perror("write");
      return EXIT_FAILURE;
  }
  
  if (close(s) == -1) {
    perror("close");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
