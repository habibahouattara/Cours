// Pour getsid définit dans unistd.h
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif
#define _XOPEN_SOURCE 500

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	pid_t pid = getpid();

	printf("Identifiants du processus, du père et de la session : %d %d %d\n",
		   pid, getppid(), getsid(0));

  
	// On crée une nouvelle session
	if (setsid() == -1) {
		perror("setsid");
		fprintf(stderr, "to be continued...\n");
		exit(EXIT_FAILURE);
	}

	printf("Identifiants du processus, de son groupe et de la session : "
		   "%d %d %d\n",
		   pid, getpgrp(),  getsid(0));

	return EXIT_SUCCESS;
}
