//  Implantation polymorphe pour la spécification ENS du TDA Ens(T) et son
//    extension ENSMIN. L'accès à l'extension ENSMIN requiert la définition de
//    la macroconstante SETMIN

#include <stdlib.h>
#include "set_mp.h"
#include "assume.h"

//  struct set, set : gestion des adresses des éléments dans l'ensemble à l'aide
//    d'un tableau, array, géré de manière préfixielle et sans doublon. Le
//    composant card mémorise la longueur du préfixe, le composant compar, la
//    fonction de comparaison des éléments à partir de leurs adresses
//  Invariant structurel pour une structure de type set correctement créée puis
//    utilisée :
//    1) card <= SET__CARD_MAX ;
//    2) l'élément pointé par xptr appartient à l'ensemble implanté si et
//      seulement s'il existe une valeur yptr dans le sous-tableau
//      array[0 ... card - 1] telle que compar(xptr, yptr) == 0
//    3) le sous-tableau array[0 ... card - 1] ne contient aucun doublon au sens
//      de compar

#define SET__CARD_MAX 100

struct set {
  int (*compar)(const void *, const void *);
  size_t card;
  const void *array[SET__CARD_MAX];
};

set *set_empty(int (*compar)(const void *, const void *)) {
  set *s = malloc(sizeof *s);
  ASSUME(s != NULL, "lack of memory space");
  s->compar = compar;
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
//    un composant de valeur égale à xptr au sens de s->compar. Renvoie
//    l'adresse du composant en cas de succès, NULL en cas d'échec
static const void **set__search(const set *s, const void *xptr) {
  for (const void * const *p = s->array; p < s->array + s->card; ++p) {
    if (s->compar(xptr, *p) == 0) {
      return (const void **) p;
    }
  }
  return NULL;
}

bool set_is_in(const set *s, const void *xptr) {
  return set__search(s, xptr) != NULL;
}

void set_add(set *s, const void *xptr) {
  if (set__search(s, xptr) != NULL) {
    return;
  }
  ASSUME(s->card < SET__CARD_MAX, "lack of memory space");
  s->array[s->card] = xptr;
  s->card += 1;
}

void set_remove(set *s, const void *xptr) {
  const void **p = set__search(s, xptr);
  if (p == NULL) {
    return;
  }
  s->card -= 1;
  *p = s->array[s->card];
}

#ifdef SETMIN

const void *set_min(const set *s) {
  ASSUME(s->card != 0, "empty set");
  const void * const *q = s->array;
  for (const void * const *p = q + 1; p < s->array + s->card; ++p) {
    if (s->compar(*q, *p) > 0) {
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
  s2->compar = s->compar;
  s2->card = s->card;
  const void **q = s2->array;
  for (const void * const *p = s->array; p < s->array + s->card; ++p) {
    *q = *p;
    ++q;
  }
  return s2;
}
