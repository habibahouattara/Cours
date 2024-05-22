#define RESULT(func, a, b)  \
  printf("%s(%u, %u) = %u\n", #func, (a), (b), (func) ((a), (b)))

RESULT(sum, 5, 8 + 4);
