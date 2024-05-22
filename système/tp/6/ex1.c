#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h> // Pour PIPE_BUF

int main(void) {
  int tube[2];
  if (pipe(tube) != 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  
  switch(fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      if (close(tube[1]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      ssize_t n;
      char buf[PIPE_BUF];
      while ((n = read(tube[0], &buf, PIPE_BUF)) > 0) {
        if (write(STDOUT_FILENO, &buf, (size_t)n) == -1) {
          perror("write");
          if (close(tube[0]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
          }
        }
      }
      if (n == -1) {
        fprintf(stderr, "pb d'écriture\n");
        if (close(tube[0]) == -1) {
          perror("close");
          exit(EXIT_FAILURE);
        }
      }
      exit(EXIT_SUCCESS);
      break;
    default:
      if (close(tube[0]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      char msg[] = "message envoyé par le pére\n";
      if (write(tube[1], &msg, sizeof(msg)) == -1) {
        perror("write");
        if (close(tube[1]) == -1) {
          perror("close");
          exit(EXIT_FAILURE);
        }
      }
      if (close(tube[1]) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      break;
  }
  return EXIT_SUCCESS;
}
