#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

#define BUF_SIZE 255

int main(void) {
  int s = socket(AF_UNIX, SOCK_STREAM, 0);
  
  struct sockaddr_un ad = {AF_UNIX, "fic"};
  
  if (connect(s, (struct sockaddr*) &ad, sizeof(ad)) == -1) {
    perror("connect");
    return EXIT_FAILURE;
  }
  
  ssize_t n;
  char t[BUF_SIZE];
  char a[BUF_SIZE];
  
  
  while ((n = read(STDIN_FILENO, t, sizeof(t))) != -1) {
    t[n] = '\0';
    write(s, t, sizeof(t));
  
    read(s, a, sizeof(a));
    
    printf("%s\n", a);
  }
  
  close(s);
  return EXIT_SUCCESS;
}
