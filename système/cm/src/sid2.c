// Pour getsid définit dans unistd.h
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	pid_t pid = getpid();

	printf("Identifiants du processus, du père et de la session : %d %d %d\n",
		   pid, getppid(), getsid(0));

	/**
	 * On remet le processus dans le groupe du père afin de pouvoir recréer
	 * un nouveau groupe lors de la création de la nouvelle session
	 **/
	if (setpgid(pid, getppid()) == -1) {
		perror("setpgid");
		exit(EXIT_FAILURE);
	}

	// On crée une nouvelle session
	if (setsid() == -1) {
		perror("setsid");
		exit(EXIT_FAILURE);
	}

	printf("Identifiants du processus, de son groupe et de la session : "
		   "%d %d %d\n",
		   pid, getpgrp(),  getsid(0));

	/**
	 * Notez que ce n'est pas la manière "classique" de creér une nouvelle
	 * session...
	 */
	
	return EXIT_SUCCESS;
}
