#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>

int main(void) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in st;
  st.sin_family = AF_INET;
  st.sin_port = htons(3264);
  inet_pton(AF_INET, "0.0.0.0", &st.sin_addr);
  
  struct sockaddr_in st_client;
  if (bind(fd, (struct sockaddr*) &st, sizeof(st)) == -1) {
    perror("bind");
    return EXIT_FAILURE;
  }
  
  listen(fd, 5);
  size_t addr_len = sockaddr_in;
  int d = fopen("message.txt", "r");
  while (1) {
    int dsv = accept(fd, (struct sockaddr*) &st_client, &addr_len);
    
    if (fork() == 0) {
      close(d);
      while ((n = read(d, buf, strlen(buf))) != 0) {
        write(dsv, buf, n);
      }
    } else {
      close(dsv);
    }
  }
}

