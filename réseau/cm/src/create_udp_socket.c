#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>

int main() {
  int socket_fd;

  // Protocole IPPROTO_UDP
  if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // Reste plus qu'à utiliser la socket ....
  while (1) ;

  return EXIT_SUCCESS;
}
