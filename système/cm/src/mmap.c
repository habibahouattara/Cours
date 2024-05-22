/**
 * On projette en mémoire le contenu d'un fichier et on affiche ce
 * qu'on a projeté.
 * Le nom du fichier doit être transmis en paramètre.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <assert.h>

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

  // On récupère la taille du fichier
  struct stat s;
  if (fstat(fd, &s) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  assert(s.st_size > 0);
  char* text = mmap(NULL, (size_t) s.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (text == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  if (write(STDOUT_FILENO, text, (size_t) s.st_size) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  /**
   * On peut aussi modifier le fichier en accédant directement à la mémoire.
   * Il faut alors ouvrir le fichier avec les droits d'écriture et projeter
   * la mémoire avec l'attribut PROT_WRITE.
   */

  /**
   * Par contre, on ne peut pas projeter plus d'octets
   * qu'il n'y en a dans le fichier.  
   */

  return EXIT_SUCCESS;
}
