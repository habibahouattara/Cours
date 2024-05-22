#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(void) {
  int fd = open("bar.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  if (close(STDOUT_FILENO) == -1) {
    perror("close(STDOUT)");
    exit(EXIT_FAILURE);
  }

  if (dup(fd) == -1) {
    perror("dup");
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
