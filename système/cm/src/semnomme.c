#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

/**
 * Un nom de sémaphore avec un identifiant pour nous assurer de son unicité.
 */
#define NOM_SEMAPHORE "/mon_semaphore_a_moi_432572985365428"

int main(void) {
  // On commence par créer notre sémaphore.
  sem_t *sem_p = sem_open(NOM_SEMAPHORE, O_RDWR | O_CREAT | O_EXCL,
                          S_IRUSR | S_IWUSR, 0);
  if (sem_p == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  // On supprime tout de suite le sémaphore pour ne pas oublier.
  // Le sémaphore ne sera réellement supprimé que quand plus
  // personne ne l'utilisera.
  if (sem_unlink(NOM_SEMAPHORE) == -1) {
    perror("sem_unlink");
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
    // On ne fait pas de sem_close car il sera réalisé automatiquement
    // à la fin du processus.
    exit(EXIT_SUCCESS);
  default:
    printf("Père : je pars faire une sieste\n");
    sleep(5);
    printf("Père : je préviens mon fils que je suis réveillé\n");
    if (sem_post(sem_p) == -1) {
      perror("sem_post");
      exit(EXIT_FAILURE);
    }
    // On ne fait pas de sem_close car il sera réalisé automatiquement
    // à la fin du processus.
    exit(EXIT_SUCCESS);
  }
}
