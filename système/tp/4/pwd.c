#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#define CURRENT_DIR "."
#define PARENT_DIR ".."

#define CLOSE_DIR(d)                                                           \
  if (closedir(d) == -1) {                                                     \
    perror("closedir");                                                        \
    exit(EXIT_FAILURE);                                                        \
  }                                                                            \

#define BUF_SIZE 1024

int main(void) {
  char *absolutepath = (char *) malloc(sizeof *absolutepath * (BUF_SIZE + 1));
  if (absolutepath == NULL) {
    fprintf(stderr, "*** erreur: plus de mémoire ***\n");
    exit(EXIT_FAILURE);
  }
  ino_t c_ino; 
  ino_t p_ino; 
  do {
    DIR *cd = opendir(CURRENT_DIR);
    if (cd == NULL) {
      perror("opendir");
      exit(EXIT_FAILURE);
    }
    struct dirent *cd_entry;
    while ((cd_entry = readdir(cd)) != NULL
        && strcmp(cd_entry->d_name, CURRENT_DIR) != 0) {
    }
    if (cd_entry == NULL) {
      fprintf(stderr, "*** erreur ***\n");
      CLOSE_DIR(cd);
      exit(EXIT_FAILURE);
    }
    DIR *pd = opendir(PARENT_DIR);
    if (pd == NULL) {
      perror("opendir");
      CLOSE_DIR(cd);
      exit(EXIT_FAILURE);
    }
    struct dirent *pd_entry;
    while ((pd_entry = readdir(pd)) != NULL
        && strcmp(pd_entry->d_name, cd_entry->d_name) != 0) {
    }
    if (pd_entry == NULL) {
      fprintf(stderr, "*** erreur ***\n");
      CLOSE_DIR(cd);
      CLOSE_DIR(pd);
      exit(EXIT_FAILURE);
    }
    if (pd_entry->d_ino != cd_entry->d_ino) {
      fprintf(stderr, "*** erreur ***\n");
      CLOSE_DIR(cd);
      CLOSE_DIR(pd);
      exit(EXIT_FAILURE);
    }
    while (sprintf(absolutepath, "/%s/%s", absolutepath,
        cd_entry->d_name) <= 0) {
      if (realloc(absolutepath, 2 * sizeof(absolutepath)) == NULL) {
        fprintf(stderr, "*** erreur: plus de mémoire ***\n");
        exit(EXIT_FAILURE);
      }
    }
    CLOSE_DIR(cd);
    CLOSE_DIR(pd);
    if (chdir(PARENT_DIR) == -1) {
      fprintf(stderr, "*** erreur ***\n");
      exit(EXIT_SUCCESS);
    }
  } while (pd_entry->d_ino != cd_entry->d_ino);
  return EXIT_SUCCESS;
}
