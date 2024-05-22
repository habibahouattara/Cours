#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Erreur d'arguments\n");
    exit(EXIT_FAILURE);
  }
  int opt;
  int off1;
  int off2;
  int flags = O_CREAT;
  while ((opt = getopt(argc, argv,"vab:e:")) != -1) {
    switch (opt) {
      case 'v':
        flags |= O_EXCL;
      case 'a':
        flags = flags | O_APPEND;
      case 'b':
        off1 = atoi(optarg);
      case 'e':
        off2 = atoi(optarg);
      default:
        fprintf(stderr, "Erreur argument mal renseigné\n");
        exit(EXIT_FAILURE);
    }
  }
  int fd1 = open(argv[2], O_RDONLY);
  printf("fd1 = %d\n", fd1);
  if (fd1 == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  int fd2 = open(argv[3], O_CREAT | flags);
  printf("fd2 = %d\n", fd2);
  if (fd2 == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  close(fd1);
  close(fd2);
  return EXIT_SUCCESS;
}
