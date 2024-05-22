#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s path\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  //if (chmod(argv[1], S_IRWXU | S_IRGRP) == -1) {
  if (chmod(argv[1], S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
    perror("chmod");
    exit(EXIT_FAILURE);
  }
  printf("Modification des droits effectuée (rwxrwxrwx)\n");

  return EXIT_SUCCESS;
}
