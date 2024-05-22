#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Erreur du nombre de paramétres\n");
		exit(EXIT_FAILURE);
	}
	
	int i = atoi(argv[1]);
	
	switch(fork()) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			printf("fils affiche les nombres de  1 à N\n");
			for (int j = 0; j < i; ++j) {
				printf("fils d'id %d affiche %d\n", getpid(), j);
			}
			
			exit(EXIT_SUCCESS);
		default:
			printf("pére affiche les nombres pairs de 1 à N\n");
			for (int k = 0; k < i; ++k) {
				printf("%d\n", (2*k));
			}
			if (wait(NULL) == -1) {
				perror("wait");
				exit(EXIT_FAILURE);
			}
	}
	return EXIT_SUCCESS;
}
