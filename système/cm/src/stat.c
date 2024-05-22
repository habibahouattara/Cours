// Pour avoir un type off_t sur 64 bits meme avec les machines 32 bits
#define _FILE_OFFSET_BITS 64

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s path\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct stat info;
  if (stat(argv[1], &info) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  printf("Taille du fichier : %ld\n", info.st_size);

  return EXIT_SUCCESS;
}
