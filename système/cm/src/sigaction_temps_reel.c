#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

void gestionnaire(int signum, siginfo_t *info, void *context);

int main(void) {
    // On construit notre action
    struct sigaction action;
    action.sa_sigaction = gestionnaire;
    action.sa_flags = SA_SIGINFO;
    if (sigfillset(&action.sa_mask) == -1) {
      perror("sigfillset");
      exit(EXIT_FAILURE);
    }
    
    // On associe l'action à tous les signaux temps-réel
    for (int i = SIGRTMIN; i <= SIGRTMAX; ++i) {
      if (sigaction(i, &action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
      }
    }
    
    printf("Signaux temps-réel : %d-%d\n", SIGRTMIN, SIGRTMAX);
    
    // On attend un signal
    // On pourrait utiliser sigwaitinfo.
    while (1) {
      sigset_t set;
      if (sigemptyset(&set) == -1) {
        perror("sigemptyset");
        exit(EXIT_FAILURE);
      }
      sigsuspend(&set);
      if (errno != EINTR) {
        perror("sigsuspend");
        exit(EXIT_FAILURE);
      }
    }
    
    return EXIT_SUCCESS; // Inutile
}

void gestionnaire(int signum, siginfo_t *info, void *context) {
  if (context != NULL) {
    // On ne se sert pas du contexte ici...
  }
  
  printf("---- Signal temps réel %d reçu.\n", signum);
  switch (info->si_code) {
  case SI_KERNEL:
    printf("Signal envoyé par le noyau.\n");
    break;
  case SI_USER:
    printf("Signal envoyé par kill (ou raise).\n");
    break;
  case SI_QUEUE:
    printf("Signal envoyé par sigqueue.\n");
    break;
  default:
    printf("Origine du signal non prise en charge par l'application\n");
    break;
  }
  printf("Uid de l'émetteur : %d\n", info->si_uid);
  printf("pid du processus émetteur : %d\n", info->si_pid);
  printf("Valeur (entier ou pointeur) : %d ou %p\n", info->si_value.sival_int, info->si_value.sival_ptr);
}
