#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set_mp.h"

typedef struct {
  const char *name;
  const char *firstname;
} id;

//  id_compar_incr_fnm_incr_nm : fonction de comparaison et procédure
//    d'affichage sur le type id. La fonction de comparaison a pour clés
//    primaire et secondaire les champs firstname et name. La procédure
//    d'affichage envoie sur la sortie standard le contenu du champ name, la
//    chaine ", ", le contenu du champ firtsname puis "\n"
static int id_compar_incr_fnm_incr_nm(const id *idptr1, const id *idptr2);
static void id_print_nm_fnm(const id *idptr);

id idarray[] = {
  {
    .name = "Ben Kalish Ezab",
    .firstname = "Abdallah",
  },
  {
    .name = "Alcazar",
    .firstname = "Peggy",
  },
  {
    .name = "Thompson",
    .firstname = "Allan",
  },
  {
    .name = "Perez",
    .firstname = "Alonzo",
  },
  {
    .name = "Haddock",
    .firstname = "Archibald",
  },
  {
    .name = "Cartoffoli",
    .firstname = "Arturo Benedetto Giovanni Giuseppe Pietro Archangelo Alfredo",
  },
  {
    .name = "Bada",
    .firstname = "Ramon",
  },
  {
    .name = "Bazaroff",
    .firstname = "Basil",
  },
  {
    .name = "Ben Mahmoud",
    .firstname = "Ali",
  },
  {
    .name = "Ben Moulfrid",
    .firstname = "Youssouf",
  },
  {
    .name = "Ben Salaad",
    .firstname = "Omar",
  },
  {
    .name = "Bergamotte",
    .firstname = "Hippolyte",
  },
  {
    .name = "Bolivar",
    .firstname = "Hippolyte",
  },
  {
    .name = "Boullu",
    .firstname = "Isidore",
  },
  {
    .name = "Charlet",
    .firstname = "Marc",
  },
  {
    .name = "Sakharine",
    .firstname = "Ivan",
  },
  {
    .name = "Zarate",
    .firstname = "Ramon",
  },
  {
    .name = "Rastapopoulos",
    .firstname = "Roberto",
  },
  {
    .name = "Tortilla",
    .firstname = "Rodrigo",
  },
};

int main(void) {
  set *s = set_empty(
    (int (*)(const void *, const void *))id_compar_incr_fnm_incr_nm);
  for (size_t k = 0; k < sizeof idarray / sizeof *idarray; ++k) {
    set_add(s, &idarray[k]);
  }
  while (!set_is_empty(s)) {
    const id *idptr = set_min(s);
    set_remove(s, idptr);
    id_print_nm_fnm(idptr);
  }
  set_dispose(&s);
  return EXIT_SUCCESS;
}

int id_compar_incr_fnm_incr_nm(const id *idptr1, const id *idptr2) {
  int c = strcmp(idptr1->firstname, idptr2->firstname);
  if (c == 0) {
    c = strcmp(idptr1->name, idptr2->name);
  }
  return c;
}

void id_print_nm_fnm(const id *idptr) {
  printf("%s, %s\n", idptr->name, idptr->firstname);
}
