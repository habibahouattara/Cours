#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define TAB_SIZE 10

typedef struct {
  int rdv;
  char tab[TAB_SIZE];
} share_t;

#define MON_SHM "/shm"
#define TAILLE_SHM sizeof(share_t)

int main(void) {
  int shm_fd = shm_open(MON_SHM, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
  if (shm_fd == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }
  if (shm_unlink(MON_SHM) == -1) {
    perror("shm_unlink");
    exit(EXIT_FAILURE);
  }
  if (ftruncate(shm_fd, TAILLE_SHM) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }
  
  share_t *share = mmap(NULL, TAILLE_SHM, PROT_READ | PROT_WRITE, MAP_SHARED,
          shm_fd, 0);
  if (share == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  
  share->rdv = 0;
  
  switch (fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      while (share->rdv == 0) {
      }
      if (write(STDOUT_FILENO, share->tab, TAB_SIZE - 1) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
      }
      if (write(STDOUT_FILENO, "\n", 1) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
      }
      exit(EXIT_SUCCESS);
    default:
      if (read(STDIN_FILENO, share->tab, TAB_SIZE - 1) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
      }
      share->rdv = 1;
      if (wait(NULL) == -1) {
        perror("wait");
        exit(EXIT_FAILURE);
      }
      break;
  }
  return EXIT_SUCCESS;
}
