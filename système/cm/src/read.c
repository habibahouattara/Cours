/**
 * Lecture d'un fichier texte et affichage sur la sortie standard.
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  char buf[BUF_SIZE + 1];
  ssize_t n;
  while ((n = read(fd, buf, BUF_SIZE)) > 0) {
    buf[n] = '\0';
    printf("%s", buf);
  }
  
  if (n == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
