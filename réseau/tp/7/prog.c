#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define BUFMAX 10000

int main(int argc, char *argv[]) {
  int n = 1;
  
  int fdE = socket(AF_INET, SOCK_DGRAM, 0);
  setsockopt(fdE, SOL_SOCKET, SO_BROADCAST, (int *) &n, sizeof(int));
  
  struct sockaddr_in addE;
  addE.sin_family = AF_INET;
  addE.sin_port = htons(2346);
  inet_pton(AF_INET, "255.255.255.255", &addE.sin_addr);
  
  int fdR = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in addR;
  addE.sin_family = AF_INET;
  addE.sin_port = htons(2346);
  inet_pton(AF_INET, "0.0.0.0", &addR.sin_addr);
  
  if (bind(fdR, (struct sockaddr*) &addR, sizeof(addR)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }
  
  char buf[BUFMAX];
  socklen_t lg = sizeof(struct sockaddr_in);
  read(STDIN_FILENO, buf, strlen(buf));
  ssize_t r;
  switch(fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      while (1) {
        recvfrom(fdR, buf, BUFMAX, 0, (struct sockaddr*) &addR, &lg);
        r = sendto(fdR, argv[1], strlen(argv[1])+1, 0, (struct sockaddr*) &addE, lg);
      }
    default:
      while (1) {
        sendto(fdE, buf, sizeof(r), 0, (struct sockaddr*) &addE, lg);
        sleep(1);
      }
  }
}
