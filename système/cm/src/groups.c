#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Affichage des groupes auxquels appartient le processus.
 */
int main(void) {
  // On récupère le nombre de groupes de processus
  int n = getgroups(0, NULL);
  if (n == -1) {
    perror("getgroups");
    exit(EXIT_FAILURE);
  }
  
  // On alloue un tableau pouvant contenir ces groupes
  gid_t groups[n];

  // On récupère les groupes
  int m;
  if ((m = getgroups(n, groups)) == -1) {
    perror("getgroups");
    exit(EXIT_FAILURE);
  }

  // On les affiche
  for (int i = 0; i < m; ++i) {
    printf("J'appartient au groupe d'identifiant %d\n", groups[i]);
  }

  exit(EXIT_SUCCESS);
}
