#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NOM_FIFO "mon_tube_nomme_7574654398"

#define NUM 20

int main(void) {
  if (mkfifo(NOM_FIFO, S_IRUSR | S_IWUSR) == -1) {
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }

  int fd;
  ssize_t n;
  int i;
  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    fd = open(NOM_FIFO, O_RDONLY);
    if (fd == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }
    while ((n = read(fd, &i, sizeof(i))) > 0) {
      printf("Lecture de %d\n", i);
    }
    if (n == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    break;
  default:
    fd = open(NOM_FIFO, O_WRONLY);
    if (fd == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }
    if (unlink(NOM_FIFO) == -1) {
      perror("unlink");
      exit(EXIT_FAILURE);
    }
    for (i = 0; i < NUM; ++i) {
      if (write(fd, &i, sizeof(i)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
      }
    }
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    break;
  }
  exit(EXIT_SUCCESS);
}
