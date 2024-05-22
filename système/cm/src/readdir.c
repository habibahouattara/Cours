#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/**
 * Lecture du contenu du répertoire de travail.
 */

int main(void) {
  DIR *dirp = opendir(".");
  if (dirp == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  errno = 0;
  struct dirent *entry;
  while ((entry = readdir(dirp)) != NULL) {
    printf("%ld %s\n", entry->d_ino, entry->d_name);
  }
  if (errno != 0) {
    perror("readdir");
    exit(EXIT_FAILURE);
  }

  if (closedir(dirp) == -1) {
    perror("closedir");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
