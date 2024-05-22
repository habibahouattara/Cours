#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	int i = 3;
	switch(fork()) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			i += 2;
			printf("fils: %d\n",i);
			exit(EXIT_SUCCESS);
		default:
			i += 1;
			printf("pére: %d\n",i);
			if (wait(NULL) == -1) {
				perror("wait");
				exit(EXIT_FAILURE);
			}
	}
	return EXIT_SUCCESS;
}
