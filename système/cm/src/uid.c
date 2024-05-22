/**
 * Pour voir quelque chose, on peut par exemple taper :
 * # su -c 'chown root uid; chmod u+s uid'
 * # ./uid
 */

// Pour seteuid
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

int main(void) {
  uid_t euid = geteuid();
  uid_t ruid = getuid();

  printf("Mes uid effectifs et réels sont : %d %d\n", euid, ruid);

  // On fixe l'UID effectif à l'UID réel
  if (seteuid(ruid) == -1) {
    perror("seteuid");
    exit(EXIT_FAILURE);
  }
  
  printf("Mes uid effectifs et réels sont : %d %d\n", geteuid(), getuid());

  // On revient en arrière
  if (seteuid(euid) == -1) {
    perror("seteuid");
    exit(EXIT_FAILURE);
  }

  printf("Mes uid effectifs et réels sont : %d %d\n", geteuid(), getuid());

  return EXIT_SUCCESS;
}
