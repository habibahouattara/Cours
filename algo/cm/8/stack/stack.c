//  Implantation polymorphe pour la spécification PILE du TDA Pile(T)

#include <stdlib.h>
#include "stack.h"

//  struct stack, stack : gestion des adresses des éléments dans la pile à
//    l'aide d'une liste dynamique simplement chainée, l'ordre dans la liste
//    étant celui inverse à l'arrivée dans la pile. Le composant head a pour
//    valeur le pointeur de tête de la liste, le composant height mémorise la
//    longueur de la liste. Le composant head repère donc la cellule associée au
//    sommet de la pile lorqu'elle n'est pas vide, et le composant height a donc
//    pour valeur la hauteur de la pile

typedef struct cstack cstack;

struct cstack {
  const void *value;
  cstack *next;
};

struct stack {
  cstack *head;
  size_t height;
};

stack *stack_empty(void) {
  stack *s = malloc(sizeof *s);
  if (s == NULL) {
    return NULL;
  }
  s->head = NULL;
  s->height = 0;
  return s;
}

const void *stack_push(stack *s, const void *xptr) {
  if (xptr == NULL) {
    return NULL;
  }
  cstack *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  p->value = xptr;
  p->next = s->head;
  s->head = p;
  s->height += 1;
  return xptr;
}

const void *stack_pop(stack *s) {
  if (s->height == 0) {
    return NULL;
  }
  cstack *p = s->head;
  s->head = p->next;
  const void *xptr = p->value;
  free(p);
  s->height -= 1;
  return xptr;
}

const void *stack_top(const stack *s) {
  if (s->height == 0) {
    return NULL;
  }
  return s->head->value;
}

bool stack_is_empty(const stack *s) {
  return s->height == 0;
}

size_t stack_height(const stack *s) {
  return s->height;
}

void stack_dispose(stack **sptr) {
  if (*sptr == NULL) {
    return;
  }
  cstack *p = (*sptr)->head;
  while (p != NULL) {
    cstack *t = p;
    p = p->next;
    free(t);
  }
  free(*sptr);
  *sptr = NULL;
}

/*
 * stack_duplicate : à implanter
 */
