#include <linux/limits.h> // Pour PIPE_BUF
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#define MUL 100

/**
 * Deux processus écrivent des blocs de données supérieurs à PIPE_BUF pendant
 * qu'un autre processus lit et affiche ces données.
 */
int main(void) {
  printf("Info : atomicité des écritures pour les tubes : %d\n", PIPE_BUF);

  int tube[2];
  if (pipe(tube) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  char buffer[2 * MUL * PIPE_BUF];
  char c;
  ssize_t n;
  ssize_t pos;

  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    if (close(tube[1]) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }

    pos = 0;
    // Il se peut que le read retourne avant d'avoir lu le nombre voulu de
    // données.
    while ((n = read(tube[0], buffer + pos,
        (size_t) (2 * MUL * PIPE_BUF - pos))) > 0) {
      fprintf(stderr, "%zd\n", n);
      pos += n;
    }
    if (n == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }    
    if (close(tube[0]) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }

    if (pos + n != 2 * MUL * PIPE_BUF) {
      fprintf(stderr, "read: nombre inattendu de données\n");
      exit(EXIT_FAILURE);
    }
    if (write(STDOUT_FILENO, buffer, 2 * MUL * PIPE_BUF) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    break;
  default:
    if (close(tube[0]) == -1) {
      perror("close");
      exit(EXIT_SUCCESS);
    }
    switch(fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      c= 'b';
      break;
    default:
      c= 'a';
      break;
    }
    // Le père et le fils écrivent dans le tube
    for (int i = 0; i < MUL * PIPE_BUF; ++i) {
      buffer[i] = c;
    }
    if (write(tube[1], buffer, MUL * PIPE_BUF) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    if (close(tube[1]) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
    break;
  }

  return EXIT_SUCCESS;
}
