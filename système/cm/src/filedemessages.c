#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>

// Des outils pour gérer l'affichage
#include "console.h"

#define NOM_FILE "/ma_file_a_moi_786432985365428"

#define NUM_ITEM 30

int main(void) {
  console_init();

  // Attributs de la file
  struct mq_attr attr;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = sizeof(int);

  // On commence par créer notre file.
  mqd_t mqd = mq_open(NOM_FILE, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, &attr);
  if (mqd == (mqd_t) -1) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  // On supprime la liste tout de suite comme ça elle
  // sera effacée même en cas d'arrêt brutal.
  if (mq_unlink(NOM_FILE) == -1) {
    perror("mq_unlink");
    exit(EXIT_FAILURE);
  }

  int i = 0;
  switch (fork()) {
  case -1:
    perror("fork");
    exit(EXIT_FAILURE);
  case 0:
    // Le fils sera le consommateur
    for (int j = 0; j < NUM_ITEM; ++j) {
      if (mq_receive(mqd,  (char*) &i, sizeof(i), NULL) == -1) {
        perror("mq_receive");
        exit(EXIT_FAILURE);
      }
      console_log("donnees consommee : %d\n", i);
    }
    break;
  default:
    // Le père sera le producteur
    for (int j = 0; j < NUM_ITEM; ++j) {
      console_log("donnees produite  : %d\n", j);
      if (mq_send(mqd, (const char *) &j, sizeof(j), 0) == -1) {
        perror("mq_send");
        exit(EXIT_FAILURE);
      }
    }
    break;
  }

  return EXIT_SUCCESS;
}
