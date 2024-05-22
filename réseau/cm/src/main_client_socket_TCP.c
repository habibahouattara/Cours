#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

#include "socket_tools.h"

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
    struct sigaction act = {
        .sa_handler = SIG_IGN,
    };
    if (sigaction(SIGPIPE, &act, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }
    
    int s;
    
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }
    
    struct sockaddr_in addr;
    build_inet_address(&addr, "localhost", "44400");
    
    if (connect(s, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        perror("connect");
        return EXIT_FAILURE;
    }
    display_inet_address(s);
    
    char buffer[BUF_SIZE];
    ssize_t n;
    while ((n = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        if (write(s, buffer, n) == -1) {
            perror("write");
            return EXIT_FAILURE;
        }
    }
    
    if (n == -1) {
        perror("read");
        return EXIT_FAILURE;
    }
  
    if (close(s) == -1) {
        perror("close");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
