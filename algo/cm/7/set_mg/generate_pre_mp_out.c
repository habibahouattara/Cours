extern void assume__fail(const char *file, long int line, const char *func,
    const char *failedtest, const char *failure);

typedef struct set set;
extern set *set_empty(void);
extern void set_add(set *s, const void *x);
extern void set_remove(set *s, const void *x);
extern _Bool set_is_empty(const set *s);
extern _Bool set_is_in(const set *s, const void *x);
extern size_t set_card(const set *s);
extern const void *set_min(const set *s);
extern void set_dispose(set **sptr);
extern set *set_duplicate(const set *s);

struct set {
  size_t card;
  const void *array[100];
};

set *set_empty(void) {
  set *s = malloc(sizeof *s);
  (s != ((void *) 0))
  ? ((void) 0)
  : assume__fail("generate_pre_mp.c", 7, __func__, "s != ((void *)0)",
      "lack of memory space");
  s->card = 0;
  return s;
}

_Bool set_is_empty(const set *s) {
  return s->card == 0;
}

size_t set_card(const set *s) {
  return s->card;
}

static const void **set__search(const set *s, const void *x) {
  for (const void * const *p = s->array; p < s->array + s->card; ++p) {
    if (THE_CMP_FUN(*p, x) == 0) {
      return (const void **) p;
    }
  }
  return (void *) 0;
}

_Bool set_is_in(const set *s, const void *x) {
  return set__search(s, x) != ((void *) 0);
}

void set_add(set *s, const void *x) {
  if (set__search(s, x) != ((void *) 0)) {
    return;
  }
  (s->card < 100)
  ? ((void) 0)
  : assume__fail("generate_pre_mp.c", 7, __func__, "s->card < 100",
      "lack of memory space");
  s->array[s->card] = x;
  s->card += 1;
}

void set_remove(set *s, const void *x) {
  const void **p = set__search(s, x);
  if (p == ((void *) 0)) {
    return;
  }
  s->card -= 1;
  *p = s->array[s->card];
}

const void *set_min(const set *s) {
  (s->card != 0)
  ? ((void) 0)
  : assume__fail("generate_pre_mp.c", 7, __func__, "s->card != 0", "empty set");
  const void * const *q = s->array;
  for (const void * const *p = q + 1; p < s->array + s->card; ++p) {
    if (THE_CMP_FUN(*p, *q) < 0) {
      q = p;
    }
  }
  return *q;
}

void set_dispose(set **sptr) {
  free(*sptr);
  *sptr = ((void *) 0);
}

set *set_duplicate(const set *s) {
  set *s2 = malloc(sizeof *s2);
  (s2 != ((void *) 0))
  ? ((void) 0)
  : assume__fail("generate_pre_mp.c", 7, __func__, "s2 != ((void *)0)",
      "lack of memory space");
  s2->card = s->card;
  const void **q = s2->array;
  for (const void * const *p = s->array; p < s->array + s->card; ++p) {
    *q = *p;
    ++q;
  }
  return s2;
}
