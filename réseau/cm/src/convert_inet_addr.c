#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <IPv4-dotted-address>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct in_addr sin_addr;
  if (inet_pton(AF_INET, argv[1], &sin_addr) <= 0) {
    fprintf(stderr, "Error when converting "
	    "IPv4 numbers-and-dots to binary form\n");
    exit(EXIT_FAILURE);
  }

  printf("IPv4 address: %s\n", inet_ntoa(sin_addr));
  
  exit(EXIT_SUCCESS);
}
