#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket_tools.h"

int main(int argc, char* argv[]) {
  if (argc > 2) {
    fprintf(stderr, "Usage: %s [host]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* host;
  if (argc == 2) {
    host = argv[1];
  } else {
    host = "localhost";
  }

  struct sockaddr_storage addr;
  socklen_t addr_size = sizeof(addr);
  build_inet_address((struct sockaddr*) &addr, &addr_size, host, "44400");

  int s;
  if ((s = socket(addr.ss_family, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  if (connect(s, (struct sockaddr*) &addr, addr_size) == -1) {
    perror("connect");
    return EXIT_FAILURE;
  }
  display_socket_address(s);

  if (write(s, "bonjour ", 9) == -1) {
    perror("write");
    return EXIT_FAILURE;
  }

  if (write(s, "tout le monde", 14) == -1) {
    perror("write");
    return EXIT_FAILURE;
  }

  sleep(1);

  if (close(s) == -1) {
    perror("close");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
