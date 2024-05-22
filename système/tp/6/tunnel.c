#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h> // Pour PIPE_BUF
#include <sys/stat.h>
#include <fcntl.h>

#define TUNNEL_IN "tunnel_in"
#define TUNNEL_OUT "tunnel_out"

int main(void) {
  if (mkfifo(TUNNEL_IN, S_IRUSR | S_IWUSR) == -1) {
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }
  
  if (mkfifo(TUNNEL_OUT, S_IRUSR | S_IWUSR) == -1) {
    perror("mkfifo");
    if (unlink(TUNNEL_IN) == -1) {
      perror("unlink");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
  }
  
  int fout = open(TUNNEL_IN, O_WRONLY);
  if (fout == -1) {
    perror("open");
    if (unlink(TUNNEL_IN) == -1) {
      perror("unlink");
      exit(EXIT_FAILURE);
    }
    if (unlink(TUNNEL_OUT) == -1) {
      perror("unlink");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
  }
  
  int fin = open(TUNNEL_OUT, O_RDONLY);
  if (fin == -1) {
    perror("open");
    if (unlink(TUNNEL_IN) == -1) {
      perror("unlink");
      exit(EXIT_FAILURE);
    }
    if (unlink(TUNNEL_OUT) == -1) {
      perror("unlink");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
  }
  
  char buf[PIPE_BUF];
  ssize_t n;
  while ((n = read(fin, &buf, PIPE_BUF)) > 0) {
    if (write(STDOUT_FILENO, &buf, (size_t)n) == -1) {
      fprintf(stderr, "pb d'écriture\n");
      if (close(fin) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      if (close(fout) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      exit(EXIT_FAILURE);
    }
  }
  
  if (n == -1) {
    fprintf(stderr, "pb de lecture\n");
    if (close(fin) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
    if (close(fout) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
  }
  
  while ((n = read(STDIN_FILENO, &buf, PIPE_BUF)) > 0) {
    if (write(fout, &buf, (size_t)n) == -1) {
      fprintf(stderr, "pb d'écriture\n");
      if (close(fin) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      if (close(fout) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
      }
      exit(EXIT_FAILURE);
    }
  }
  
  if (n == -1) {
    fprintf(stderr, "pb de lecture\n");
    if (close(fin) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
    if (close(fout) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
  }
  
  if (close(fin) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
  if (close(fout) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
