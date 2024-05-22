// Pour ftruncate
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

/**
 * Un nom de segment de mémoire partagé avec un identifiant pour nous assurer de son unicité.
 */
#define NOM_SHM "/mon_shm_a_moi_674272985365427"
#define TAILLE_SHM sizeof(sem_t)

int main(void) {
  // On commence par créer un segment de mémoire partagée pour stocker notre sémaphore
  int shm_fd = shm_open(NOM_SHM, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
  if (shm_fd == -1) {
    perror("shm_open");
    exit(EXIT_SUCCESS);
  }

  if (shm_unlink(NOM_SHM) == -1) {
    perror("shm_unlink");
    exit(EXIT_FAILURE);
  }

  if (ftruncate(shm_fd, TAILLE_SHM) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }

  // On projette notre shm en mémoire et on lui associe une variable de type sémaphore
  sem_t *sem_p = mmap(NULL, TAILLE_SHM, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (sem_p == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  // On initialise le sémaphore
  if (sem_init(sem_p, 1, 0) == -1) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  // On se duplique
  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    printf("Fils : j'attend papa\n");
    if (sem_wait(sem_p) == -1) {
      perror("sem_wait");
      exit(EXIT_FAILURE);
    }
    printf("Fils : au revoir\n");
    // Plus personne n'a besoin du sémaphore : je le détruis
    if (sem_destroy(sem_p) == -1) {
      perror("sem_destroy");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
  default:
    printf("Père : je pars faire une sieste\n");
    sleep(5);
    printf("Père : je préviens mon fils que je suis réveillé\n");
    if (sem_post(sem_p) == -1) {
      perror("sem_post");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
  }

  // À la fin du (des) processus, le descripteur du shm ainsi
  // que la projection mémoire sont automatiquement détruits.
}
