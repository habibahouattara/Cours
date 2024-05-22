#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "hashtable.h"
#include "holdall.h"
#include "lists.h"

//--- Autour de la fonction principale -----------------------------------------

//  structure permettant la gestion de toutes les options
typedef enum {
  OPT_HELP, OPT_TABLE, OPT_LIMIT, OPT_ORDER, OPT_SEED,
  OPT__NBR
} option;

struct {
  char *label;
  char *descr;
} opt_value[OPT__NBR] = {
  [OPT_HELP] = {
    .label = "-h",
    .descr = "display the help for this program"
  },
  [OPT_TABLE] = {
    .label = "-t",
    .descr = "removes the normal display and replaces it with the table display"
  },
  [OPT_LIMIT] = {
    .label = "-l",
    .descr = "fix the limit on the number of words to be produced at NUMBER"
  },
  [OPT_ORDER] = {
    .label = "-n",
    .descr = "fix the order of the generation to NUMBER"
  },
  [OPT_SEED] = {
    .label = "-s",
    .descr = "fix the germ of the random generator to NUMBER at the beginning"
  },
};

//--- fonctions statics --------------------------------------------------------

//  lists_rfree : libére l'espace alloué pour la valeur pointé par ptr pour le
//    type holdall *hashword (mot quelconque)
static int rfree(void *ptr);

//  lists_rfree : libére l'espace alloué pour la valeur pointé par ptr pour le
//    type holdall *lists ** dans le fourre-tout associé (hashkey, hashlist)
static int lists_rfree(void *ptr);

//  srcptr_display : affiche la liste des clés pointé par key et ses valeurs
//    associées pointé par value avec l'affichage de la table
static int display_table_value_key(lists *key, lists *value);

//  random_select_word : choisi aléatoirement un mot dans la liste associée à la
//    clé (pointé par keys) dans la table pointé par ht. Renvoie NULL si le mot
//    n'existe pas, affecer la valeur trouvé à v sinon
static void *random_select_word(lists *succ, hashtable *ht, lists *keys);

//  display_help : affiche l'aide et termine le programme
static void display_help(const char *progname);

//  display_try_help : affiche un message d'aide pour utiliser l'aide et termine
//    le programme
static void display_try_help(const char *progname);

//--- fonction principale ------------------------------------------------------

#define STR(s)  #s
#define XSTR(s) STR(s)
#define STRING_LENGTH_MAX        31
#define WORD_LIMIT_MAX           1000
#define WORD_LIMIT_MIN           100
#define ORDER_LINIT_MIN          1
#define ORDER_LIMIT_MAX          5
#define SEED_LIMIT_MIN           0
#define SEED_LIMIT_MAX           10

int main(int argc, char *argv[]) {
  srand(0);
  //  b : variable booléenne pour savoir si on affiche la table ou pas
  bool b = false;
  //  m == nombre de mots à produire initialisé à 100
  int m = 100;
  //  n == ordre de la génération par défault à 1
  int n = 1;
  long int x = 0;
  char *endptr;
  errno = 0;
  
  for (int k = 1; k < argc; ++k) {
    if (strcmp(argv[k], "-h") == 0) {
      display_help(argv[0]);
    }
    if (strcmp(argv[k], "-t") == 0 ) {
      b = true;
    }
    if (strcmp(argv[k], "-l") == 0) {
      x = strtol(argv[k+1], &endptr, 10);
      if (*endptr != '\0'&& errno != 0) {
        fprintf(stderr, "*** Error: parameter '%s' invalid.\n", argv[k+1]);
        display_try_help(argv[0]);
        return EXIT_FAILURE;
      }
      if (! (x >= WORD_LIMIT_MIN && x <= WORD_LIMIT_MAX)) {
        fprintf(stderr, "*** Error: limit '%ld' is out of the limits.\n", x);
        display_try_help(argv[0]);
        return EXIT_FAILURE;
      } else {
        m = (int) x;
      }
    }
    if (strcmp(argv[k], "-n") == 0) {
      x = strtol(argv[k+1], &endptr, 10);
      if (*endptr != '\0' && errno != 0) {
        fprintf(stderr, "*** Error: parameter '%s' invalid.\n", argv[k+1]);
        display_try_help(argv[0]);
        return EXIT_FAILURE;
      }
      if (! (x >= ORDER_LINIT_MIN && x <= ORDER_LIMIT_MAX)) {
        fprintf(stderr, "*** Error: order '%ld' is out of the limits.\n", x);
        display_try_help(argv[0]);
        return EXIT_FAILURE;
      } else {
        n = (int) x;
      }
    }
    if (strcmp(argv[k], "-s") == 0) {
      x = strtol(argv[k+1], &endptr, 10);
      if (*endptr != '\0' && errno != 0) {
        fprintf(stderr, "*** Error: parameter '%s' invalid.\n", argv[k+1]);
        display_try_help(argv[0]);
        return EXIT_FAILURE;
      }
      if (! (x >= SEED_LIMIT_MIN && x <= SEED_LIMIT_MAX)) {
        fprintf(stderr, "*** Error: seed '%ld' is out of the limits.\n", x);
        display_try_help(argv[0]);
        return EXIT_FAILURE;
      } else {
        srand((unsigned int) x);
      }
    }
  }
  option k = 0;
  const char *a = argv[1];
  while (k < OPT__NBR && strcmp(a, opt_value[k].label) != 0) {
    ++k;
  }
  if (k == OPT__NBR) {
    fprintf(stderr, "*** Error: parameter '%s' not found.\n", a);
    display_try_help(argv[0]);
    return EXIT_FAILURE;
  }
  
  //----- PHASE DE LECTURE ET DE CONSTRUCTION DE LA TABLE -----//
  
  hashtable *ht = hashtable_empty((int (*)(const void *, const void *))
      lists_compar, (size_t (*)(const void *)) lists_value_hashfun);
  //  hashword : tout mes mots
  holdall *hashword = holdall_empty(); 
  //  hashkey : que les clé sans répétition
  holdall *hashkey = holdall_empty(); 
  //  hashlist : les listes des valeurs et les répétions de clés
  holdall *hashlist = holdall_empty();

  //  d = u0,u1,..,un-1
  char *d = " ";
  //  keys : liste de n-uplet de mots initialisé à vide
  lists *keys = lists_key_empty((size_t) n, d);

  if (ht == NULL || hashword == NULL || keys == NULL || hashkey == NULL ||
      hashlist == NULL) {
    goto error_capacity;
  }
  //  string : contient les mots lu sur l'entré
  char string[STRING_LENGTH_MAX + 1];
  //  v : mot qui sera utilisé pour former la clé suivante dans la liste
  //  associée à la clé(u0,u1,..,un-1)
  char *v;
  //  succ : liste associée à celle des clés(u0,u1,..,un-1)
  lists *succ;

  do {
    //  affecte à succ le résultat de la recherche dans la table de la clé keys
    succ = (lists *) hashtable_value(ht, keys);
    //  si succ == NULL donc la clé n'a pas été trouvé alors initialisation de
    //    succ au vide
    if (succ == NULL) {
      succ = lists_empty();
      //  si on n'a pas réussi é crée la liste libérer l'espace pointé par keys
      if (succ == NULL) {
        lists_dispose(&keys);
        goto error_capacity;
      }
      //  si on n'a pas réussi à mettre succ dans le fourre-tout associé aux
      //    listes de successeur on libére l'espace pointé par succ et keys
      if (holdall_put(hashlist, succ) != 0) {
        lists_dispose(&succ);
        lists_dispose(&keys);
        goto error_capacity;
      }
      //  si on n'a pas réussi à mettre keys dans le fourre-tout associé aux
      //    listes de clé on libére l'espace pointé par keys
      if (holdall_put(hashkey, keys) != 0) {
        lists_dispose(&keys);
        goto error_capacity;
      }
      //  si on n'a pas réussi à ajouter le couple (keys/succ) dans la table on
      //    sort du programme
      if (hashtable_add(ht, keys, succ) == NULL) {
        goto error_capacity;
      }
    } else {
      //  si on n'a pas réussi à mettre keys dans le fourre-tout associé aux
      //    listes de successeur on libére l'espace pointé par keys
      if (holdall_put(hashlist, keys) != 0) {
        lists_dispose(&keys);
        goto error_capacity;
      }
    }
    //  si on n'a plus de mot à lire
    if (scanf("%" XSTR(STRING_LENGTH_MAX) "s", string) != 1) {
      v = d;
    } else {
      //  le mot lu sur l'entré (string) et affecter à v avec strcpy
      v = malloc(strlen(string) + 1);
      if (v == NULL) {
        goto error_capacity;
      }
      strcpy(v, string);
      //  si on n'a pas réussi à mettre v dans le fourre-tout associé aux mots
      //    on libére l'espace pointé par v
      if (holdall_put(hashword, v) != 0) {
        free(v);
        goto error_capacity;
      }
      //  remplace u0,u1n..,un-1 par u1,..,un-1,v
      keys = lists_new_createdlist(keys, v);
      if (keys == NULL) {
        goto error_replaced;
      }
    }
    //  ajout du mot v dans la liste des successeurs
    if (lists_insert_tail(succ, v) != 0) {
      goto error_insert;
    }
  } while (strcmp(v, d) != 0);

  //----- PHASE DE GENERATION ALEATOIRE -----//
  if (b) {
    if (holdall_apply_context(hashkey,
        (int (*)(void *, void *)) display_table_value_key,
        (void *(*)(void *, void *)) hashtable_value, ht) != 0) {
      goto error_write;
    }
  } else {
    //  keys : liste de n-uplet de mots initialisé à vide
    keys = lists_key_empty((size_t) n, d);
    if (holdall_put(hashlist, keys) != 0) {
      lists_dispose(&keys);
      goto error_capacity;
    }
    //  v contient un mot aléatoire choisit dans la liste des successeurs
    v = random_select_word(succ, ht, keys);
    //  i == nombre de mots produits
    int i = 0;
    while ((strcmp(v, d) != 0) && i < m) {
      if (fprintf(stdout, " %s", v) < 0) {
        goto error_write;
      }
      //  remplace u0,u1n..,un-1 par u1,..,un-1,v
      keys = lists_new_createdlist(keys, v);
      if (keys == NULL) {
        goto error_replaced;
      }
      if (holdall_put(hashlist, keys) != 0) {
        lists_dispose(&keys);
        goto error_capacity;
      }
      v = random_select_word(succ, ht, keys);
      ++i;
    }
  }
  fprintf(stdout, "\n");
  int r = EXIT_SUCCESS;
  goto dispose;
error_capacity:
  fprintf(stderr, "*** Error: not enough memory.\n");
  r = EXIT_FAILURE;
  goto dispose;
error_write:
  fprintf(stderr, "*** Error: the random text has not been written.\n");
  r = EXIT_FAILURE;
  goto dispose;
error_insert:
  fprintf(stderr, "*** Error: the word has not been inserted.\n");
  r = EXIT_FAILURE;
  goto dispose;
error_replaced:
  fprintf(stderr, "*** Error: the list has not been replaced.\n");
  r = EXIT_FAILURE;
  goto dispose;
dispose:
  hashtable_dispose(&ht);
  if (hashword != NULL) {
    holdall_apply(hashword, rfree);
  }
  if (hashkey != NULL) {
    holdall_apply(hashkey, lists_rfree);
  }
  if (hashlist != NULL) {
    holdall_apply(hashlist, lists_rfree);
  }
  holdall_dispose(&hashword);
  holdall_dispose(&hashkey);
  holdall_dispose(&hashlist);
  return r;
}

//--- Développement ------------------------------------------------------------

#define FUN_FAILURE -1
#define FUN_SUCCESS 0

int rfree(void *ptr) {
  free(ptr);
  return FUN_SUCCESS;
}

int lists_rfree(void *ptr) {
  lists_dispose((lists **) &ptr);
  return FUN_SUCCESS;
}

int display_table_value_key(lists *key, lists *value) {
  printf("\n");
  lists_display(key, stdout);
  lists_display(value, stdout);
  return FUN_SUCCESS;
}

void *random_select_word(lists *succ, hashtable *ht, lists *keys) {
  succ = (lists *) hashtable_value(ht, keys);
  if (succ == NULL) {
    fprintf(stderr, "*** Error: not found in the table.\n");
  }
  char *v = (char *) lists_index_to_value(succ, rand()
      % (int) lists_length(succ));
  return v;
}

#define USG_OPT "OPTIONs"
#define USG_SRC "TXT"
#define USG_TXT "text based on the statistical properties of a source text"
#define USG_NOTE "-l X, -n X and -s X where X is a strictly positive numeric"

void display_help(const char *progname) {
  fputs("Usage : ", stdout);
  fputs(progname, stdout);
  fputs(" " USG_OPT " " USG_SRC "\n", stdout);
  fputs("Generate a random " USG_TXT "\n", stdout);
  fputs(USG_OPT " can be : \n", stdout);
  for (option k = 0; k < OPT__NBR; ++k) {
    fputs(opt_value[k].label, stdout);
    fputs("\t", stdout);
    fputs(opt_value[k].descr, stdout);
    fputs("\n", stdout);
  }
  fputs("\n", stdout);
  fputs("NOTES :\n", stdout);
  fputs("1) The options -l, -n and -s have to be use like this : \n", stdout);
  fputs(" " USG_NOTE " variable\n", stdout);
  printf("2) -l has to be between %d and %d.\n", WORD_LIMIT_MIN, WORD_LIMIT_MAX);
  printf("3) -n has to be between %d and %d.\n", ORDER_LINIT_MIN,
      ORDER_LIMIT_MAX);
  printf("4) -s has to be between %d and %d.\n", SEED_LIMIT_MIN, SEED_LIMIT_MAX);
  exit(EXIT_SUCCESS);
}

void display_try_help(const char *progname) {
  fprintf(stderr, "You can try « %s -h » for more information.\n",
      progname);
  exit(EXIT_FAILURE);
}
