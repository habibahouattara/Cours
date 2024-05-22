#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s signum pid value\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int signum = atoi(argv[1]);
  pid_t pid = atoi(argv[2]);
  union sigval value;
  value.sival_int = atoi(argv[3]);

  if (sigqueue(pid, signum, value) == -1) {
    perror("sigqueue");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
