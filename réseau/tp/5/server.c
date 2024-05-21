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
  
  struct sockaddr_un adS = { AF_UNIX, "ficS" };
  
  if (bind(s, (struct sockaddr*) &adS, sizeof(adS)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }
  
  char buf[1024];
  
  struct sockaddr_un adC;
  
  socklen_t lg = sizeof(struct sockaddr_un);
  
  int n;
  
  while (1) {
    n = recvfrom(s, buf, sizeof(buf) - 1, 0, (struct sockaddr*) &adC, &lg);
    buf[n] = '\0';
    sendto(s, buf, sizeof(buf), 0, (struct sockaddr*) &adC, lg);
  }
  close(s);
  return EXIT_SUCCESS;
}

void gestionnaire(int signum) {
  if (unlink("ficS") == -1) {
    perror("unlink");
    return;
  }
}
