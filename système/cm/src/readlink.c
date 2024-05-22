// Pour symlink et readlink
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 512
#define LINK_NAME "/home/yguesnet/Université/Enseignement/SE/Cours/src/readlink.test"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  /* Création du lien symbolique */
  if (symlink(argv[1], LINK_NAME) == -1) {
    perror("symlink");
    exit(EXIT_FAILURE);
  }

  /* Un tampon pour recevoir le contenu du lien. */
  /* On initialise le tampon à 0 car readlink ne met pas de 0 terminal */
  char buf[BUF_SIZE + 1];
  memset(buf, 0, sizeof(buf));

  if (readlink(LINK_NAME, buf, BUF_SIZE) == -1) {
    perror("readlink");
    exit(EXIT_FAILURE);
  }
  printf("Contenu du lien : %s\n", buf);
  
  return EXIT_SUCCESS;
}
