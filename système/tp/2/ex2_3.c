#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Erreur nombre de paramétres\n");
		exit(EXIT_FAILURE);
	}
		
	switch(fork()) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			execvp(argv[1], argv+1);
			perror("exec");
			exit(EXIT_SUCCESS);
		default:
			if (wait(NULL) == -1) {
				perror("wait");
				exit(EXIT_FAILURE);
			}
			printf("Merci\n");
	}
	return EXIT_SUCCESS;
}
