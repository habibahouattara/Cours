//  Implantation spécialisée au type int pour la spécification ENS du TDA Ens(T)
//    et son extension ENSMIN. L'accès à l'extension ENSMIN requiert la
//    définition de la macroconstante SETMIN

#include <stdlib.h>
#include "set_m_int.h"
#include "assume.h"

//  struct set, set : gestion des éléments dans l'ensemble à l'aide d'un
//    tableau, array, géré de manière préfixielle et sans doublon. Le composant
//    card mémorise la longueur du préfixe
//  Invariant structurel pour une structure type set correctement créée puis
//    utilisée :
//    1) card <= SET_CARD_MAX ;
//    2) x appartient à l'ensemble implanté si et seulement si x possède une
//      occurrence dans le sous-tableau array[0 ... card - 1] ;
//    3) le sous-tableau array[0 ... card - 1] ne contient aucun doublon

#define SET__CARD_MAX 100

struct set {
  size_t card;
  int array[SET__CARD_MAX];
};

set *set_empty(void) {
  set *s = malloc(sizeof *s);
  ASSUME(s != NULL, "lack of memory space");
  s->card = 0;
  return s;
}

bool set_is_empty(const set *s) {
  return s->card == 0;
}

size_t set_card(const set *s) {
  return s->card;
}

//  set__search : recherche dans le sous-tableau s->array[0 ... s->card - 1]
//    un composant de valeur égale à x. Renvoie l'adresse du composant en cas de
//    succès, NULL en cas d'échec
static int *set__search(const set *s, int x) {
  for (const int *p = s->array; p < s->array + s->card; ++p) {
    if (x == *p) {
      return (int *) p;
    }
  }
  return NULL;
}

bool set_is_in(const set *s, int x) {
  return set__search(s, x) != NULL;
}

void set_add(set *s, int x) {
  if (set__search(s, x) != NULL) {
    return;
  }
  ASSUME(s->card < SET__CARD_MAX, "lack of memory space");
  s->array[s->card] = x;
  s->card += 1;
}

void set_remove(set *s, int x) {
  int *p = set__search(s, x);
  if (p == NULL) {
    return;
  }
  s->card -= 1;
  *p = s->array[s->card];
}

#ifdef SETMIN

int set_min(const set *s) {
  ASSUME(s->card != 0, "empty set");
  const int *q = s->array;
  for (const int *p = q + 1; p < s->array + s->card; ++p) {
    if (*q > *p) {
      q = p;
    }
  }
  return *q;
}

#endif

void set_dispose(set **sptr) {
  free(*sptr);
  *sptr = NULL;
}

set *set_duplicate(const set *s) {
  set *s2 = malloc(sizeof *s2);
  ASSUME(s2 != NULL, "lack of memory space");
  s2->card = s->card;
  int *q = s2->array;
  for (const int *p = s->array; p < s->array + s->card; ++p) {
    *q = *p;
    ++q;
  }
  return s2;
}
