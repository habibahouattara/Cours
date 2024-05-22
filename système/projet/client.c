#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>

#include "conf.h"

void gestionnaire(int signum);

int main(int argc, char *argv[]) {
  //--- PHASE DE SINGAUX -------------------------------------------------------
  
  struct sigaction action;
  action.sa_handler = gestionnaire;
  action.sa_flags = 0;
  if (sigfillset(&action.sa_mask) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }
  
  for (int i = 1; i < 32; ++i) {
    if (i != SIGKILL && i != SIGCHLD && i != SIGSTOP
        && sigaction(i, &action, NULL) == -1) {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  }
  
  
  //--- PHASE DE CONVERSION DES ARGUMENTS ARGV[1] À ARGV[ARGC - 1] -------------
  
  // tab: tableau qui contiendra les caractères entrés sur la ligne de commande
  //    argv[1] à argv[argc - 1]
  int tab[argc - 2];
  
  char *ptr;
  errno = 0;
  
  for (int i = 0; i < argc - 2; ++i) {
    // k: contient la valeur converti en base 10
    int k = (int) strtol(argv[i + 1], &ptr, 10);
    // si la conversion s'est bien passée, place à l'indice i la valeur converti
    // sinon une erreur est apparue et on quitte l'application
    if (*ptr == '\0' && errno == 0) {
      tab[i] = k;
    }
    else
    {
      fprintf(stderr, "*** conversion echouée pour: %s***\n", argv[i + 1]);
      return EXIT_FAILURE;
    }
  }
  
  //--- PHASE D'INITIALISATION DE LA REQUÊTE -----------------------------------
  
  request_t req;
  req.size = sizeof(tab) / sizeof(*tab); // taille du tableau
  strcpy(req.ope, argv[argc - 1]); // dernier argument, donc l'opération
  req.pid = getpid();
  
  //--- PHASE DE CRÉATION DU SEGMENT DE MÉMOIRE PARTAGÉE -----------------------
  
  // seg_name: nom du segment de mémoire, doit être de la forme /pid où pid est
  //    le pid du client qui envoie la requête
  char segment_name[SEGMENT_NAME_SIZE];
  sprintf(segment_name, "/%d", req.pid);
  
  // création et ouverture
  // si une erreur survient quitter l'application
  int fd_shm = shm_open(segment_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR
      | S_IWUSR);
  if (fd_shm == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }
  
  // définition de la taille du shm
  if (ftruncate(fd_shm, SHM_SIZE * sizeof(req.size)) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }
  
  //--- PHASE DE PROJECTION DU SEGMENT -----------------------------------------
  
  // projection de la mémoire avec l'atribut PROT_WRITE pour qu'on puisse écrire
  //    le contenu du tableau tab dedans
  int* ptr_shm = mmap(NULL, SHM_SIZE * sizeof(req.size), PROT_WRITE |
      PROT_READ, MAP_SHARED, fd_shm, 0);
  if (ptr_shm == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  
  // copie dans la mémoire du contenu du tableau tab
  memcpy(ptr_shm, &tab, SHM_SIZE * sizeof(req.size));
  
  //--- PHASE DE CRÉATION DU TUBE NOMMÉ ----------------------------------------
  
  // création du tube nommé
  if (mkfifo(TUBE, S_IRUSR | S_IWUSR) == -1) {
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }
  
  // ouverture du tube en mode écriture
  int d = open(TUBE, O_WRONLY);
  if (d == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  
  //--- PHASE DE L'ÉCRITURE DE LA REQUÊTE --------------------------------------
  
  // écriture de la requête dans le tube
  if (write(d, (request_t *) &req, sizeof(req)) == -1) {
    fprintf(stderr, "*** write échoué ***\n");
    if (close(d) == -1) {
      perror("close");
      exit(EXIT_FAILURE);
    }
    exit(EXIT_FAILURE);
  }
  
  //--- PHASE DE LECTURE DE LA RÉSPONSE DU SERVEUR -----------------------------
  
  // ouverture du shm avec l'attribut PROT_READ
  // blocage du client avant lecture dans le shm
  
  for (size_t i = 0; i < req.size; ++i) {
    printf("%d\n", ptr_shm[i]);
  }
  
  return EXIT_SUCCESS;
}

void gestionnaire(int signum) {
  if (signum < 0) {
    fprintf(stderr, "Mauvais signal\n");
    exit(EXIT_FAILURE);
  }
  
  printf("\nSignal reçu n° %d\n", signum);
  
  if (unlink(TUBE) == -1) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }
  
  printf("Au revoir\n");
  
  exit(EXIT_SUCCESS);
}
