//  Simple linked list

#include <stdlib.h>
#include <stdio.h>

struct cell {
  int value;
  struct cell *next;
};

int display(struct cell *head);
int insert_tail(struct cell **headptr, int value);
int kill(struct cell *head);

int main(void) {
  struct cell *head = NULL;
  insert_tail(&head, 14);
  insert_tail(&head, 18);
  insert_tail(&head, 39);
  insert_tail(&head, 45);
  for (int k = 100; k > 0; --k) {
    insert_tail(&head, k);
  }
  display(head);
  kill(head);
  return EXIT_SUCCESS;
}

#define FUN_SUCCESS 0
#define FUN_FAILURE 1

int display(struct cell *head) {
  struct cell *p = head;
  while (p != NULL) {
    printf("%s%d", p == head ? "" : " ", p->value);
    p = p->next;
  }
  printf("\n");
  return FUN_SUCCESS;
}

int insert_tail(struct cell **headptr, int value) {
  struct cell *q = malloc(sizeof *q);
  if (q == NULL) {
    return FUN_FAILURE;
  }
  q->value = value;
  q->next = NULL;
  if (*headptr == NULL) {
    *headptr = q;
  } else {
    struct cell *p = *headptr;
    while (p->next != NULL) {
      p = p->next;
    }
    p->next = q;
  }
  return FUN_SUCCESS;
}

int kill(struct cell *head) {
  struct cell *p = head;
  while (p != NULL) {
    struct cell *t = p;
    p = p->next;
    free(t);
  }
  return FUN_SUCCESS;
}
