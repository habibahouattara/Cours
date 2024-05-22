#include "console.h"

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void console_init(void) {
  int fd = open(CONSOLE_NOM_FICHIER, O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, 0600);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  if (dup2(fd, STDOUT_FILENO) == -1) {
    perror("dup2");
    exit(EXIT_FAILURE);
  }
  if (close(fd)) {
    perror("close");
    exit(EXIT_FAILURE);
  }
}

char msg[CONSOLE_MSG_MAX_SIZE];

void console_log(const char *format, ...) {
  va_list args;
  va_start(args, format);
  
  vsnprintf(msg, sizeof(msg), format, args);
  if (write(STDOUT_FILENO, msg, strlen(msg)) != (ssize_t) strlen(msg)) {
    perror("write");
    exit(EXIT_SUCCESS);
  }
  va_end(args);
}
