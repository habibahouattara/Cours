#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "heure.h"


void affiche_heure_local() {
	time_t t = time(NULL);
	if (t == (time_t) -1) {
		perror("time");
		exit(EXIT_FAILURE);
	}
	struct tm *h = localtime(&t);
	if (h == NULL) {
		perror("localtime");
		exit(EXIT_FAILURE);
	}
	printf("%d:%d:%d\n", h->tm_hour, h->tm_min, h->tm_sec);
}

void affiche_heure_gmt() {
	time_t t = time(NULL);
	if (t == (time_t) -1) {
		perror("time");
		exit(EXIT_FAILURE);
	}
	struct tm *h = gmtime(&t);
	if (h == NULL) {
		perror("gmtime");
		exit(EXIT_FAILURE);
	}
	printf("%d:%d:%d\n", h->tm_hour, h->tm_min, h->tm_sec);
}

char *ascheure(const struct tm *tmptr) {
}
