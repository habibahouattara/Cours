#include <stdlib.h>
#include <stdio.h>
#include <sys/fcntl.h> // open, O_WRONLY, ...
#include <sys/stat.h> // S_IRUSR, S_IWUSR, ...

int main(void) {
  /**
   * On crée un fichier foo dans le répertoire courant
   * en donnant les droits de lecture-écriture-exécution au propriétaire
   * et de lecture-écriture au groupe et aux autres.
   */
  int fd = open("foo", O_WRONLY | O_CREAT | O_EXCL,
                S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  printf("Nouveau fichier ouvert, descripteur %d\n", fd);

  return EXIT_SUCCESS;
}
