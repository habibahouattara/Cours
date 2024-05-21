#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>

int main(void) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in std;
  std.sin_family = AF_INET;
  std.sin_port = htons(3264);
  std.sin_addr.s_addr = INADDR_ANY;
  
  inet_pton(AF_INET, "192.168.36.4", &std.sin_addr);
  
  if (connect(fd, (struct sockaddr*) &std, sizeof(std)) == -1) {
    perror("connect");
    return EXIT_FAILURE;
  }
  
  char file[] = "message.txt";
  char buf[1024];
  
  send(fd, sizeof(buf), strlen(buf) + 1, 0);
  send(fd, file, strlen(file) + 1, 0);
  
  FILE *f = fopen("message.txt", "w");
  
  if (f == NULL) {
		perror("error file");
		return EXIT_FAILURE;
	}
	
	while (recv(fd, buf, sizeof(buf), 0) > 0) {
		fprintf(f, "%s", buf);
	}
	
	fclose(f);
	
	if (close(fd) == -1) {
		perror("close fd");
		return EXIT_FAILURE;
	}
}
