/**
 * Lecture d'un fichier texte et affichage sur la sortie standard.
 * On peut créer un fichier de test avec :
 * dd if=/dev/urandom of=test.bin bs=1k count=100000
 */

// Pour clock_gettime()
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define BUF_SIZE 1024

void read_file(int fd, size_t buf_size);
void fread_file(FILE* fs, size_t buf_size);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  FILE* fs = fopen(argv[1], "r");
  if (fs == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  printf("Lecture caractère par caractère\n");
  read_file(fd, 1);
 
  printf("Lecture caractère par caractère avec fread (avec un tampon de %d octets)\n", BUFSIZ);
  fread_file(fs, 1);

  if (lseek(fd, 0, SEEK_SET) == (off_t) -1) {
    perror("lseek");
    exit(EXIT_FAILURE);
  }

  printf("Lecture par blocs de %d octets\n", BUF_SIZE);
  read_file(fd, BUF_SIZE);


  if (close(fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }

  if (fclose(fs) == EOF) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}

void read_file(int fd, size_t buf_size) {
  char buf[BUF_SIZE + 1];

  struct timespec start;
  if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
    perror("clock_gettime");
    exit(EXIT_FAILURE);
  }

  ssize_t n;
  while ((n = read(fd, buf, buf_size)) > 0) {
	  //buf[n] = 0;
  }
  
  struct timespec end;
  if (clock_gettime(CLOCK_REALTIME, &end) == -1) {
    perror("clock_gettime");
    exit(EXIT_FAILURE);
  }

  if (end.tv_nsec < start.tv_nsec) {
    --end.tv_sec;
    end.tv_nsec += 1000000000;
  }
  time_t sec = end.tv_sec - start.tv_sec;
  long nsec = end.tv_nsec - start.tv_nsec;

  long diff_ms = sec * 1000 + nsec / 1000000;
  printf("Temps écoulé : %ld ms\n", diff_ms);

  if (n == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }
}

void fread_file(FILE* fs, size_t buf_size) {
  char buf[BUF_SIZE + 1];

  struct timespec start;
  if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
    perror("clock_gettime");
    exit(EXIT_FAILURE);
  }

  size_t n;
  while ((n = fread(buf, buf_size, 1, fs)) > 0) {
	  //buf[n] = 0;
  }

  if (ferror(fs) != 0) {
    perror("fread");
    exit(EXIT_FAILURE);
  }
  
  struct timespec end;
  if (clock_gettime(CLOCK_REALTIME, &end) == -1) {
    perror("clock_gettime");
    exit(EXIT_FAILURE);
  }

  if (end.tv_nsec < start.tv_nsec) {
    --end.tv_sec;
    end.tv_nsec += 1000000000;
  }
  time_t sec = end.tv_sec - start.tv_sec;
  long nsec = end.tv_nsec - start.tv_nsec;

  long diff_ms = sec * 1000 + nsec / 1000000;
  printf("Temps écoulé : %ld ms\n", diff_ms);
}
