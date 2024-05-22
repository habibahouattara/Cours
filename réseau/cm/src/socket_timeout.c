#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>

#define BUF_SIZE 1024

int connect_socket(const char *server_name, const char *server_service);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s server_name port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int s = connect_socket(argv[1], argv[2]);
    printf("Connecté.\n");

    char buffer[BUF_SIZE];
    while (fgets(buffer, BUF_SIZE, stdin) != NULL) {
        if (write(s, buffer, strlen(buffer) + 1) == -1) {
            perror("write");
            return EXIT_FAILURE;
        }
        char rcv_buf[BUF_SIZE];
        if (read(s, rcv_buf, BUF_SIZE) == -1) {
            perror("read");
            return EXIT_FAILURE;
        }
        printf("%s", rcv_buf);
    }
    
    if (close(s) == -1) {
        perror("close");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

int connect_socket(const char *server_name, const char *server_service) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *res;
    int err;
    if ((err = getaddrinfo(server_name, server_service, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }
    if (res == NULL) {
        fprintf(stderr, "getaddrinfo: empty address list\n");
        exit(EXIT_FAILURE);
    }

    int s;
    if ((s = socket(res->ai_family, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    struct timeval timeout = {
        .tv_sec = 1,
        .tv_usec = 0,
    };
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Sert aussi à connect
    if (setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    if (connect(s, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
    return s;
}

