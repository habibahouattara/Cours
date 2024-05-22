#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

int main(int argc, char* argv[]) {
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s service protocole\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char* proto = NULL;
  if (argc == 3) {
    proto = argv[2];
  }

  struct servent* servent = getservbyname(argv[1], proto);
  if (servent == NULL) {
    fprintf(stderr, "Unable to find the service\n");
    exit(EXIT_FAILURE);
  }

  printf("%s (%s) port number : %d\n", servent->s_name, servent->s_proto, ntohs(servent->s_port));
  exit(EXIT_SUCCESS);
}
