#define _GNU_SOURCE         // Pour syscall

// Pour clock_gettime()
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/time.h>

#define NLOOP 10000000

int foo(void);

int main(void) {
	srandom((unsigned int) time(NULL));
	
	struct timespec start;
	if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
		perror("clock_gettime");
		exit(EXIT_FAILURE);
	}

	double d = 0;
	for (int k = 0; k < NLOOP; ++k) {
		//d += foo();
		d += (int) time(NULL); // vDSO
	}
	
	struct timespec end;
	if (clock_gettime(CLOCK_REALTIME, &end) == -1) {
		perror("clock_gettime");
		exit(EXIT_FAILURE);
	}
	
	if (end.tv_nsec < start.tv_nsec) {
		--end.tv_sec;
		end.tv_nsec += 1000000000;
	}
	time_t sec = end.tv_sec - start.tv_sec;
	long nsec = end.tv_nsec - start.tv_nsec;
	
	long diff_ms = sec * 1000 + nsec / 1000000;
	printf("Temps écoulé : %ld ms\n", diff_ms);

	printf("d = %lf\n", d);

	if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
		perror("clock_gettime");
		exit(EXIT_FAILURE);
	}

	d = 0;
	for (int k = 0; k < NLOOP; ++k) {
		d += (int) syscall(SYS_getpid);
		//d += getpid();
		//struct timeval tv;
	    //d += gettimeofday(&tv, NULL); // vDSO
		//d += (int) syscall(SYS_gettimeofday, &tv, NULL);
	}
	
	if (clock_gettime(CLOCK_REALTIME, &end) == -1) {
		perror("clock_gettime");
		exit(EXIT_FAILURE);
	}
	
	if (end.tv_nsec < start.tv_nsec) {
		--end.tv_sec;
		end.tv_nsec += 1000000000;
	}
	sec = end.tv_sec - start.tv_sec;
	nsec = end.tv_nsec - start.tv_nsec;
	
	diff_ms = sec * 1000 + nsec / 1000000;
	printf("Temps écoulé : %ld ms\n", diff_ms);

	printf("d = %lf\n", d);

	return EXIT_SUCCESS;
}

int foo(void) {
	return (int) random();
}
