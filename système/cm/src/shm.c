// Pour ftruncate
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

/**
 * Un nom de segment de mémoire partagé avec un identifiant pour nous assurer
 * de son unicité.
 */
#define NOM_SHM "/mon_shm_a_moi_589422985365427"
#define TAILLE_SHM sizeof(int)

int main(void) {
  // On commence par créer un segment de mémoire partagée pour stocker
  //notre variable
  int shm_fd = shm_open(NOM_SHM, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
  if (shm_fd == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  // On supprime tout de suite le segment pour ne pas oublier.
  // Le segment ne sera réellement supprimé que quand plus
  // personne ne l'utilisera.
  /**
  if (shm_unlink(NOM_SHM) == -1) {
    perror("shm_unlink");
    exit(EXIT_FAILURE);
  }
  */
  
  // On fixe la taille de notre shm.
  if (ftruncate(shm_fd, TAILLE_SHM) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }

  // On projette notre variable partagée en mémoire.
  // On récupère ainsi l'adresse de notre variable partagée
  volatile int *rdv = mmap(NULL, TAILLE_SHM, PROT_READ | PROT_WRITE,
                           MAP_SHARED, shm_fd, 0);
  if (rdv == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  // On n'a plus besoin du descripteur du le shm
  if (close(shm_fd) == -1) {
    perror("close");
    exit(EXIT_FAILURE);
  }
  
  // Le rendez-vous est atteint lorsque la variable vaut 1
  *rdv = 0;

  // On se duplique
  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    printf("Fils : j'attend papa\n");
    while (*rdv == 0) ; // Rigolo non ?
    printf("Fils : au revoir\n");
    exit(EXIT_SUCCESS);
  default:
    printf("Père : je pars faire une sieste\n");
    sleep(5);
    printf("Père : je préviens mon fils que je suis réveillé\n");
    *rdv = 1;
    exit(EXIT_SUCCESS);
  }

  // À la fin du (des) processus,
  // la projection mémoire est automatiquement détruite.
}
