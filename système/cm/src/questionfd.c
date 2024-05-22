#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(void) {
  int fd = open("foo.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    if (write(fd, "fiston", 6) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    break;
  default:
    if (wait(NULL) == -1) {
      perror("wait");
      exit(EXIT_FAILURE);
    }
    if (write(fd, "papa", 4) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    break;
  }

  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
