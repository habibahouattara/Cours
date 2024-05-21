#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <signal.h>

void gestionnaire(int signum);

int main(void) {
  // signal
  struct sigaction action;
  action.sa_handler = gestionnaire;
  action.sa_flags = 0;
  
  if (sigfillset(&action.sa_mask) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }
  
  if (sigaction(SIGINT, &action, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
  
  int s = socket(AF_UNIX, SOCK_DGRAM, 0);
  
  struct timeval tps;
  tps.tv_sec = 1;
  tps.tv_usec = 0;
  
  setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*) &tps, sizeof(struct timeval));
  
  struct sockaddr_un adC = { AF_UNIX, "ficC" };
  
  if (bind(s, (struct sockaddr*) &adC, sizeof(adC)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }
  
  struct sockaddr_un adS = { AF_UNIX, "ficS" };
  
  char buf[1024];
  
  socklen_t lg = sizeof(struct sockaddr_un);
  
  ssize_t n;
  
  while ((n = (read(STDIN_FILENO, buf, sizeof(buf) - 1))) != -1) {
    buf[n-1] = '\0';
    sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &adS, lg);
    recvfrom(s, buf, strlen(buf), 0, (struct sockaddr*) &adS, &lg);
    printf("%s", buf);
  }
  close(s);
  return EXIT_SUCCESS;
}

void gestionnaire(int signum) {
  if (unlink("ficC") == -1) {
    perror("unlink");
  }
  printf("...\n");
}
