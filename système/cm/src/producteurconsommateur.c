#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

/**
 * Un nom de segment de mémoire partagé avec un identifiant pour nous assurer de son unicité.
 */
#define NOM_SHM "/mon_shm_a_moi_589422985365427"

// Taille de notre tampon
#define N 10

// Nos variables en mémoire partagée.
struct fifo {
  sem_t mutex;
  sem_t vide;
  sem_t plein;
  size_t tete;      // Position d'ajout dans le tampon
  size_t queue;     // Position de suppression dans le tampon
  char buffer[]; // Le tampon contenant les données
};

// L'en-tête du segment de mémoire partagée
struct fifo *fifo_p = NULL;

/**
 * On met dans notre shm :
 * - une ent-ête contenant toutes les variables permettant la manipulation du tampon ;
 * - un tampon de N caractères.
 */
#define TAILLE_SHM (sizeof(struct fifo) + N)

void ajouter(char c);
char retirer(void);

/**
 * Envoie au consommateur les caractères qu'il lit sur l'entrée standard.
 */
void producteur(void);


/**
 * Affiche ce que le producteur lui envoie sur la sortie standard.
 * S'arrete dès qu'il lit un 0.
 */
void consommateur(void);

int main(void) {
  // On commence par créer un segment de mémoire partagée
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

  char *shm_ptr = mmap(NULL, TAILLE_SHM, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  fifo_p = (struct fifo *) shm_ptr;

  // Initialisation des variables
  if (sem_init(&fifo_p->mutex, 1, 1) == -1) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  if (sem_init(&fifo_p->vide, 1, N) == -1) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  if (sem_init(&fifo_p->plein, 1, 0) == -1) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }

  fifo_p->tete = 0;
  fifo_p->queue = 0;

  // On se duplique
  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    consommateur();
    exit(EXIT_SUCCESS);
  default:
    producteur();
  }

  if (wait(NULL) == -1) {
    perror("wait");
    exit(EXIT_FAILURE);
  }

  // Il faudrait prendre des précautions particulières afin d'être sûr
  // que les sémaphores sont détruits à la fin du processus (gestion des signaux).
  if (sem_destroy(&fifo_p->mutex) == -1) {
    perror("sem_destroy");
    exit(EXIT_FAILURE);
  }
  if (sem_destroy(&fifo_p->plein) == -1) {
    perror("sem_destroy");
    exit(EXIT_FAILURE);
  }
  if (sem_destroy(&fifo_p->vide) == -1) {
    perror("sem_destroy");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

/**
 * Contrairement au support du cours,
 * la synchronisation est effectuée dans ajouter
 * et non pas dans producteur.
 */
void producteur(void) {
  ssize_t n;
  char c;
  while ((n = read(STDIN_FILENO, &c, 1)) > 0) {
    ajouter(c);
  }
  if (n == -1)  {
    perror("read");
    exit(EXIT_FAILURE);
  }

  ajouter(0);
}

/**
 * Contrairement au support du cours,
 * la synchronisation est effectuée dans retirer
 * et non pas dans consommateur.
 */
void consommateur(void) {
  char c;
  do {
    c = retirer();
    errno = 0;
    if (write(STDOUT_FILENO, &c, 1) != 1) {
      if (errno != 0) {
        perror("write");
        exit(EXIT_FAILURE);
      } else {
        fprintf(stderr, "write: impossible d'écrire les données demandées");
        exit(EXIT_FAILURE);
      }
    }

  } while (c != 0);
}

void ajouter(char c) {
    if (sem_wait(&fifo_p->vide) == -1) {
      perror("sem_wait");
      exit(EXIT_FAILURE);
    }
    if (sem_wait(&fifo_p->mutex) == -1) {
      perror("sem_wait");
      exit(EXIT_FAILURE);
    }

    fifo_p->buffer[fifo_p->tete] = c;
    fifo_p->tete = (fifo_p->tete + 1) % N;

    if (sem_post(&fifo_p->mutex) == -1) {
      perror("sem_post");
      exit(EXIT_FAILURE);
    }
    if (sem_post(&fifo_p->plein) == -1) {
      perror("sem_post");
      exit(EXIT_FAILURE);
    }
}

char retirer(void) {
    if (sem_wait(&fifo_p->plein) == -1) {
      perror("sem_wait");
      exit(EXIT_FAILURE);
    }
    if (sem_wait(&fifo_p->mutex) == -1) {
      perror("sem_wait");
      exit(EXIT_FAILURE);
    }

    char c = fifo_p->buffer[fifo_p->queue];
    fifo_p->queue = (fifo_p->queue + 1) % N;

    if (sem_post(&fifo_p->mutex) == -1) {
      perror("sem_post");
      exit(EXIT_FAILURE);
    }
    if (sem_post(&fifo_p->vide) == -1) {
      perror("sem_post");
      exit(EXIT_FAILURE);
    }

    return c;
}

