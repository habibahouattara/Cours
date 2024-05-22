#include <sys/socket.h>
#include <sys/un.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
  // Initialisation à la C99
  struct sockaddr_un adresse_unix = {
    .sun_family = AF_UNIX,           // sun_family
    .sun_path   = "un_chemin.socket" // sun_path
  };
  
  // Initialisation d'une variable déjà déclarée
  memset(&adresse_unix, '\0', sizeof(struct sockaddr_un));
  adresse_unix.sun_family = AF_UNIX;
  strncpy(adresse_unix.sun_path, "un_chemin.socket", sizeof(adresse_unix.sun_path) - 1);

  // Une autre adresse
  memset(&adresse_unix, '\0', sizeof(struct sockaddr_un));
  adresse_unix.sun_family = AF_UNIX;
  // On laisse un 0 devant l'adresse...
  strncpy(adresse_unix.sun_path + 1, "une adresse bizarre", sizeof(adresse_unix.sun_path) - 1);

  return EXIT_SUCCESS;
}
