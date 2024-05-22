#ifndef CONF__H
#define CONF__H

#ifndef OPE_TAB_SIZE
#define OPE_TAB_SIZE 10
#endif

#ifndef SEGMENT_NAME_SIZE
#define SEGMENT_NAME_SIZE 255
#endif

#ifndef SHM_SIZE
#define SHM_SIZE sizeof(int)
#endif

#define TUBE "tube"

typedef struct {
  char ope[OPE_TAB_SIZE];
  size_t size;
  pid_t pid;
} request_t;

#endif
