#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[]) {
    printf("Arguments de la commande :\n");
    for (int i = 0; i < argc; ++i) {
      printf("  argv[%d] = %s\n", i, argv[i]);
    }

    printf("Variables d'environnement :\n");
    for (int i = 0; environ[i] != NULL; ++i) {
      printf("  environ[%d] = %s\n", i, environ[i]);
    }

    return EXIT_SUCCESS;
}
