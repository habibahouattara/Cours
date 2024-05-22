/**
 * Lecture d'un fichier texte et affichage sur la sortie standard.
 * On peut créer un fichier de test avec :
 * dd if=/dev/urandom of=test.bin bs=1k count=1000
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

void fread_file(const char *filename, size_t buf_size, size_t nmemb);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  fread_file(argv[1], 1, 1);
  fread_file(argv[1], 1024, 1);
  //fread_file(argv[1], 1024, 1024);
  //fread_file(argv[1], 1, 1024);

  return EXIT_SUCCESS;
}

void fread_file(const char *filename, size_t buf_size, size_t nmemb) {
  printf("Lecture par blocs de %zu éléments avec un tampon de %zu octets\n",
		 nmemb, buf_size);

  FILE* fs = fopen(filename, "rb");
  if (fs == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  char buf[buf_size];
  if (setvbuf(fs, buf, _IOFBF, buf_size) != 0) {
	  fprintf(stderr, "setvbuf: failed\n");
	  exit(EXIT_FAILURE);
  }
  
  struct timespec start;
  if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
    perror("clock_gettime");
    exit(EXIT_FAILURE);
  }

  char x[nmemb];
  size_t n;
  while ((n = fread(&x, 1, nmemb, fs)) > 0) {
  }

  if (ferror(fs) != 0) {
    perror("fread");
    exit(EXIT_FAILURE);
  }

  if (fclose(fs) != 0) {
    perror("fclose");
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
