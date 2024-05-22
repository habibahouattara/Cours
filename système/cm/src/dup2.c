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

  if (dup2(fd, STDOUT_FILENO) == -1) {
    perror("dup2");
    exit(EXIT_FAILURE);
  }

  if (close(fd) == -1) {
    perror("close(fd)");
    exit(EXIT_FAILURE);
  }

  const char * const argv[] = {"ps", "-j", NULL};
  execvp("ps", (char* const *) argv);
  perror("exevp");
  exit(EXIT_FAILURE);
}
