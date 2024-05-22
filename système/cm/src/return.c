#include <stdlib.h>
#include <stdio.h>

/**
 * Petit programme qui se termine en retournant la valeur qui lui a été
 * transmise en paramètre.
 * Cette valeur peut être récupérée dans le shell au travers de la variable $?
 */
int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s retval\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	int retval = atoi(argv[1]);
	return retval;
}
