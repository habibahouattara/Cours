#include <stdlib.h>
#include <stdio.h>

#define INSTR_A0(x) printf("A0 %d\n", x)
#define INSTR_A1(x) printf("A1 %d\n", x)
#define INSTR_A2(x) printf("A2 %d\n", x)

#define COND(x) ((x) == 0)
#define MAP(x)  (x)
#define REAL(x) ((x) - 1)

int fun(int x) {
  INSTR_A0(x);
  if (COND(x)) {
    INSTR_A1(x);
    return MAP(x);
  } else {
    INSTR_A2(x);
    return fun(REAL(x));
  }
}

int main(void) {
  fun(5);
}
