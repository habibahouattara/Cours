/**
 * Utilisation de ftruncate pour redimensionner la sortie standard.
 * La taille doit être transmise en paramètre.
 * Evidemment la sortie standard doit être rédirigé vers un fichier
 * régulier.
 */

#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

// Macro définie dans Large File Support extension (LFS)
#ifndef _FILE_OFFSET_BITS
// Sur les machines 32 bits, utilise 64 bits pour off_t
#define _FILE_OFFSET_BITS 64
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s size\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  off_t size = atol(argv[1]);
  if (ftruncate(STDOUT_FILENO, size) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
