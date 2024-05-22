//  Interface polymorphe pour la spécification TABLE du TDA Table(T, T')

#include <stdlib.h>
#include "table.h"

//  struct table, table : gestion par liste dynamique simplement chainées. Le
//    composant compar mémorise la fonction de comparaison des clés. Le
//    composant head a pour valeur le pointeur de tête de la liste

//  Si la macroconstante INSERT_TAIL est définie, l'insertion dans la liste a
//    lieu en queue. Dans la cas contraire, elle a lieu en tête

typedef struct cell cell;

struct cell {
  const void *keyptr;
  const void *valptr;
  cell *next;
};

struct table {
  int (*compar)(const void *, const void *);
  cell *head;
};

table *table_empty(int (*compar)(const void *, const void *)) {
  table *t = malloc(sizeof *t);
  if (t == NULL) {
    return NULL;
  }
  t->compar = compar;
  t->head = NULL;
  return t;
}

//  table__search : recherche dans la liste de la table associée à t la première
//    occurrence d'une clé égale à celle d'adresse keyptr au sens de compar.
//    Renvoi l'adresse du pointeur qui repère la cellule qui contient cette
//    occurrence si elle existe. Renvoi sinon l'adresse du pointeur qui marque
//    la fin de la liste sinon
cell **table__search(const table *t, const void *keyptr) {
  cell * const *pp = &t->head;
  while (*pp != NULL && t->compar((*pp)->keyptr, keyptr) != 0) {
    pp = &((*pp)->next);
  }
  return (cell **) pp;
}

const void *table_add(table *t, const void *keyptr, const void *valptr) {
  if (valptr == NULL) {
    return NULL;
  }
  cell **pp = table__search(t, keyptr);
  if (*pp != NULL) {
    (*pp)->valptr = valptr;
  } else {
    cell *p = malloc(sizeof *p);
    if (p == NULL) {
      return NULL;
    }
    p->keyptr = keyptr;
    p->valptr = valptr;
#ifndef INSERT_TAIL
    p->next = t->head;
    t->head = p;
#else
    p->next = NULL;
    *pp = p;
#endif
  }
  return valptr;
}

const void *table_remove(table *t, const void *keyptr) {
  cell **pp = table__search(t, keyptr);
  if (*pp == NULL) {
    return NULL;
  }
  cell *p = *pp;
  const void *valptr = p->valptr;
  *pp = p->next;
  free(p);
  return valptr;
}

const void *table_search(const table *t, const void *keyptr) {
  cell *p = *table__search(t, keyptr);
  return p == NULL ? NULL : p->valptr;
}

void table_dispose(table **tptr) {
  if (*tptr == NULL) {
    return;
  }
  cell *p = (*tptr)->head;
  while (p != NULL) {
    cell *t = p;
    p = p->next;
    free(t);
  }
  free(*tptr);
  *tptr = NULL;
}
