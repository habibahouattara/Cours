#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "heure.h"

int main(int argc, char *argv[]) {
	if (argc > 2) {
		printf("Trop d'argument");
		exit(EXIT_FAILURE);
	}
	if ((strcmp(argv[1], "-l")) == 0) {
		affiche_heure_local();
	} else if ((strcmp(argv[1], "-g") == 0)) {
		affiche_heure_gmt();
	}
	return EXIT_SUCCESS;
}
