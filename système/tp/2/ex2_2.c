#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Erreur nombre de paramétres\n");
		exit(EXIT_FAILURE);
	}
	
	int i = atoi(argv[1]);
	
	switch(fork()) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			for (int j = 0; j <= i; ++j) {
				printf("processus n° %d\n", j);
			}
			exit(EXIT_SUCCESS);
		default:
			if (wait(NULL) == -1) {
				perror("wait");
				exit(EXIT_FAILURE);
			}
	}
	
	return EXIT_SUCCESS;
}

