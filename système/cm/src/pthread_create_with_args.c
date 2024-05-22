#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Exemple de création d'un thread avec des arguments.
 */

/*
 * Les arguments transmis au thread.
 * On utilise ici une structure incomplète (ce qui complique
 * un peu les choses) car on souhaite transmettre une chaine
 * dont on ne connait pas a priori la taille maximale.
 */
struct my_thread_args {
  int num; // Un numéro de lancement pour le thread
  char s[]; // Une chaine de caractères
};

void * run(struct my_thread_args* a);

typedef void *(*start_routine_type)(void *);

int main(int argc, char * argv[]) {
  if (argc == 1) {
    fprintf(stderr, "Usage: %s arg1 [ arg2 ... ]\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  for (int i = 1; i < argc; ++i) {
    int errnum;
    pthread_t th;

    struct my_thread_args* a = malloc(sizeof(struct my_thread_args)
									  + strlen(argv[i]) + 1);
    if (a == NULL) {
      perror("malloc()");
      exit(EXIT_FAILURE);
    }
    a->num = i;
    strcpy(a->s, argv[i]);
    
    if ((errnum = pthread_create(&th, NULL,
								 (start_routine_type) run, a)) != 0) {
      fprintf(stderr, "pthread_create: %s\n", strerror(errnum));
      exit(EXIT_FAILURE);
    }
  }
  
  pthread_exit(NULL);
}

void * run(struct my_thread_args* a) {
  printf("Thread numéro %i avec la chaine \"%s\"\n", a->num, a->s);

  // On libère la mémoire allouée par le thread principal
  free(a);
  
  return NULL;
}
