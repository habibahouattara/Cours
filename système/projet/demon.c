#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>
#include <signal.h>

#include "conf.h"

// arguments transmis au thread
struct thread_arg {
  int numTh; // numéro pour le thread
  size_t size;
  pid_t pid;
  char ope[OPE_TAB_SIZE];
  char shm_name[SEGMENT_NAME_SIZE];
};

// routine que les threads vont exécuter
void * run(struct thread_arg* a);

// modification du type de la routine
typedef void *(*start_routine_type)(void *);

void gestionnaire(int signum);

int main(void) {
  //--- ACTION -----------------------------------------------------------------
  
  struct sigaction action;
  action.sa_handler = gestionnaire;
  action.sa_flags = 0;
  
  if (sigfillset(&action.sa_mask) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }
  
  for (int i = 1; i < 32; ++i) {
    if (i != SIGKILL && i != SIGCHLD && i != SIGSEGV && i != SIGSTOP
        && sigaction(i, &action, NULL) == -1) {
      perror("sigaction");
      exit(EXIT_FAILURE);
    }
  }
  
  //--- PHASE D'OUVERTURE DU TUBE ----------------------------------------------
  
  // ouverture du tube en mode lecture
  // si une erreur survient, suppression du tube et quitter l'application
  int d = open(TUBE, O_RDONLY);
  if (d == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  
  //--- PHASE DE LECTURE DU TUBE ET CRÉATION DES THREADS -----------------------
  
  ssize_t n;
  request_t req;
  
  // le serveur boucle sur la lecture du tube
  while ((n = read(d, (request_t *) &req, sizeof(req))) > 0 ) {
    // si on sort de la boucle c'est qu'on a pas réussi à lire
    if (n == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    
    // si on a résussi la lecture
    if (n > 0) {
      int errnum;
      
      // th_tab: tableau de thread de la taille du tableau (celui passé dans le
      //    segment de mémoire partagée)
      pthread_t th_tab[req.size - 1];
      
      // le serveur crée un processus
      // - le fils crée les n-1 threads
      // - le père est en attente de lire une autre requête dans le tube
      switch(fork()) {
        case -1:
          perror("fork");
          exit(EXIT_FAILURE);
        case 0:
          for (size_t k = 0; k < req.size; ++k) {
            // création de n structure 
            struct thread_arg* a = malloc(sizeof(struct thread_arg));
            if (a == NULL) {
              perror("malloc");
              exit(EXIT_FAILURE);
            }
            
            // initialisation des arguments de la structure qui va être passée
            //    en paramètre des threads
            a->numTh = (int) k;
            sprintf(a->shm_name, "/%d", req.pid);
            a->size = req.size;
            a->pid = req.pid;
            strcpy(a->ope, req.ope);
            
            // création de n thread: chaque structure aura son thread
            if ((errnum = pthread_create(&th_tab[k], NULL,
                (start_routine_type) run, a)) != 0) {
              fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
              exit(EXIT_FAILURE);
            }
          }
          
          // attente de la fin des n threads
          for (size_t k = 0; k < req.size; ++k) {
            if ((errnum = pthread_join(th_tab[k], NULL)) != 0) {
              fprintf(stderr, "pthread_join: %s\n", strerror(errnum));
              exit(EXIT_FAILURE);
            }
          }
          exit(EXIT_SUCCESS);
        default:
          if (wait(NULL) == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
          }
      }
    }
  }
  
  pthread_exit(NULL);
}

void * run(struct thread_arg* a) {
  int p = a->numTh;
  
  // chaque thread va ouvrir le shm et lire dedans
  int fd_shm = shm_open(a->shm_name, O_RDWR, S_IRUSR | S_IWUSR);
  if (fd_shm == -1) {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }
  
  // mémoire partagée avec l'attribut PROT_READ
  int* ptr_shm = mmap(NULL, SHM_SIZE * sizeof(a->size), PROT_READ | PROT_WRITE, 
      MAP_SHARED, fd_shm, 0);
  if (ptr_shm == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  
  // t: tableau sur lequel on va faire le calcul de la somme préfixe
  int t[a->size];
  
  // copie du contenu du shm dans le tableayu t
  for (size_t i = 0; i < a->size; ++i) {
    memcpy(&t[i], &ptr_shm[i], a->size);
  }
  
  size_t log2_of_N = (size_t) log2((double) a->size);
  
  // tableau temporaire qui contiendra les nouvelles valeurs calculé au tour
  //    précédent
  int temp[a->size];
  
  for (size_t d = 1; d <= log2_of_N; ++d) {
    // si le numéro du thread est au plus une puissance de 2
    // faire le calcule et place le résultat dans le nouveau tableau
    if (p >= (int) pow(2, (int)d - 1)) {
      t[p] = t[p] + t[p - (int) pow(2, (int)d - 1)];
      temp[p] = t[p];
    }
    
    // si ce n'est pas au plus une puissance de 2
    // on place juste la valeur dans le nouveau tableau
    if (p < (int) pow(2, (int)d - 1)) {
      temp[p] = t[p];
    }
    
    //printf("d = %ld, temp[%d] = %d\n", d, p, temp[p]);
  }
  
  for (size_t i = 0; i < a->size; ++i) {
    memcpy(&ptr_shm[i], &temp[i], a->size);
  }
  
  // libération de la mémoire allouée par le thread principal
  free(a);
  
  return NULL;
}

void gestionnaire(int signum) {
  printf("\nFermeture serveur: signal %d reçu\n", signum);
  
  exit(EXIT_SUCCESS);
}
