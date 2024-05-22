#include "lists.h"
#include <stdlib.h>
#include <string.h>

//--- Types clists et lists ----------------------------------------------------

//  struct cldigs, cldigs : structure capable de gérer une liste dynamique
//    simplement chainée sur le type lists
typedef struct clists clists;

struct clists {
  const void *value;
  clists *next;
};

struct lists {
  clists *head;
  clists *tail;
  size_t length;
};

//--- Raccourcis ---------------------------------------------------------------

#define FUN_FAILURE -1
#define FUN_SUCCESS 0

#define MALLOC(p)                                                              \
  (p) = malloc(sizeof *(p));

#define FREE(p)                                                                \
  free(p);                                                                     \
  (p) = NULL;

//--- Fonctions static ---------------------------------------------------------

//  duplicate : crée une structure de données dont la liste qu'elle représente
//    contient les mêmes éléments que celle associé s. Renvoie NULL en cas de
//    dépassement de capacité. Renvoie un pointeur vers l'objet qui gère la
//    structure de données créée sinon
static lists *duplicate(lists *s) {
  lists *q = lists_empty();
  if (q == NULL) {
    return NULL;
  }
  clists *p = s->head;
  clists **pp = &(q->head);
  while (p != NULL) {
    MALLOC(*pp);
    if (*pp == NULL) {
      lists_dispose(&q);
      return NULL;
    }
    (*pp)->value = p->value;
    // mets dans la queue la derniere valeur allouer
    q->tail = *pp;
    pp = &((*pp)->next);
    p = p->next;
  }
  *pp = NULL;
  q->length = s->length;
  return q;
}

//  delete_first_element : supprime l'élément de tête de la liste associé à s.
//    Renvoie une valeur positive en cas de succès, une valeur négative sinon
static int delete_first_element(lists *s) {
  if (lists_is_empty(s)) {
    return FUN_FAILURE;
  }
  clists *t = s->head;
  s->head = t->next;
  free(t);
  if (s->head == NULL) {
    s->tail = NULL;
  }
  s->length -= 1;
  return FUN_SUCCESS;
}

//--- Fonctions lists ----------------------------------------------------------

lists *lists_empty(void) {
  lists *s;
  MALLOC(s);
  if (s == NULL) {
    return NULL;
  }
  s->head = NULL;
  s->tail = NULL;
  s->length = 0;
  return s;
}

bool lists_is_empty(const lists *s) {
  return s->length == 0;
}

size_t lists_length(const lists *s) {
  return s->length;
}

lists *lists_key_empty(size_t n, const void *ptr) {
  lists *s = lists_empty();
  if (s == NULL) {
    return NULL;
  }
  clists **pp = &(s->head);
  while (s->length < n) {
    MALLOC(*pp);
    if (*pp == NULL) {
      lists_dispose(&s);
      return NULL;
    }
    (*pp)->value = ptr;
    s->tail = *pp;
    pp = &((*pp)->next);
    s->length += 1;
  }
  *pp = NULL;
  return s;
}

int lists_insert_tail(lists *s, const void *ptr) {
  clists *t;
  MALLOC(t);
  if (t == NULL) {
    return FUN_FAILURE;
  }
  t->value = ptr;
  t->next = NULL;
  if(s->head == NULL) {
    s->head = t;
  } else {
    s->tail->next = t;
  }
  s->tail = t;
  s->length += 1;
  return FUN_SUCCESS;
}

int lists_compar(const lists *s1, const lists *s2) {
  clists *t1 = s1->head;
  clists *t2 = s2->head;
  int d = strcmp(t1->value, t2->value);
  while (d == 0 && t1 != NULL && t2 != NULL) {
    d = strcmp(t1->value, t2->value);
    t1 = t1->next;
    t2 = t2->next;
  }
  return (d != 0) ? d : (t2 == NULL) - (t1 == NULL);
}

int lists_display(const lists *s, FILE *stream) {
  clists *t = s->head;
  while (t != NULL) {
    if (fprintf(stream, "[%s] ", (char *) t->value) < 0) {
      return FUN_FAILURE;
    }
    t = t->next;
  }
  return FUN_SUCCESS;
}

size_t lists_value_hashfun(const lists *s) {
  clists *t = s->head;
  size_t h = 0;
  while (t != NULL) {
    const char *c = (char *) t->value;
    for (const unsigned char *p = (const unsigned char *) c; *p != '\0'; ++p) {
      h = 37 * h + *p;
    }
    t = t->next;
  }
  return h;
}

void *lists_index_to_value(lists *s, int n) {
  int k = 0;
  clists *t = s->head;
  while (t != NULL && k < n) {
    t = t->next;
    ++k;
  }
  return (void *) t->value;
}

lists *lists_new_createdlist(lists *s, const void *ptr) {
  lists *t = duplicate(s);
  if (t == NULL) {
    return NULL;
  }
  if (delete_first_element(t) != 0) {
    return NULL;
  }
  if (lists_insert_tail(t, ptr) != 0) {
    return NULL;
  }
  return t;
}

void lists_dispose(lists **sptr) {
  if (*sptr == NULL) {
    return;
  }
  clists *p = (*sptr)->head;
  while (p != NULL) {
    clists *t = p;
    p = p->next;
    free(t);
  }
  FREE(*sptr);
}
