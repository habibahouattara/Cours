#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <unistd.h>

#define BACKLOG 0
#define TIMEOUT_SEC 8
#define BUF_SIZE 512

int create_socket_server(const char *host, const char*port);
void accept_new_service(int socket_srv, fd_set *readset, int *nfds);
void echo(int socket_fd, fd_set *readset);
void process_connections(int socket_srv);
void wait_select(int nfds, fd_set *readset_res);

int main(int argc, char **argv)  {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s host_name port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int socket_srv = create_socket_server(argv[1], argv[2]);

    process_connections(socket_srv);
    
    return EXIT_FAILURE;
}

int create_socket_server(const char *host, const char*port) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *res;
    int err;
    if ((err = getaddrinfo(host, port, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }

    if (res == NULL) {
        fprintf(stderr, "getaddrinfo: empty address list\n");
        exit(EXIT_FAILURE);
    }
    
    int socket_srv;
    if ((socket_srv = socket(res->ai_family, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    if (bind(socket_srv, res->ai_addr, res->ai_addrlen) != 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

    if (listen(socket_srv, BACKLOG) != 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return socket_srv;
}

void accept_new_service(int socket_srv, fd_set *readset, int *nfds) {
    int s;
    if ((s = accept(socket_srv, NULL, NULL)) == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    FD_SET(s, readset);
    *nfds = s + 1 > *nfds ? s + 1 : *nfds;                    
}

void echo(int socket_fd, fd_set *readset) {
    ssize_t n;
    char buf[BUF_SIZE];
    if ((n = read(socket_fd, buf, BUF_SIZE)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    if (n == 0) {
        printf("fd=%d: close\n", socket_fd);
        if (close(socket_fd) != 0) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        FD_CLR(socket_fd, readset);
        // On pourrait également mettre à jour nfds...
    } else {
        sleep(6);
        printf("fd=%d: envoi de \"%s\"\n", socket_fd, buf);
        if (write(socket_fd, buf, (size_t) n) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        // On pourrait s'assurer qu'on a bien réussi à écrire tous les octets
    }
}

void process_connections(int socket_srv) {
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(socket_srv, &readset);
    int nfds = socket_srv + 1;

    do {
        // POSIX nous dit que fd_set est un entier ou une structure, on doit donc pouvoir la copier
        fd_set readset_res = readset; 
        wait_select(nfds, &readset_res);

        for (int i = 0; i < nfds; ++i) {
            if (FD_ISSET(i, &readset_res)) {
                if (i == socket_srv) {
                    accept_new_service(socket_srv, &readset, &nfds);
                } else {
                    echo(i, &readset);
                }
            }
        }
    } while (1);
}

void wait_select(int nfds, fd_set *readset_res) {
    // Ici cela peut paraître étrange de fixer un timeout pour un serveur
    // mais c'est juste pour donner un exemple...
    /*
    struct timeval timeout = {
        .tv_sec = TIMEOUT_SEC,
        .tv_usec = 0,
    };
    */
    int r;
    //r = select(nfds, readset_res, NULL, NULL, &timeout);
    r = select(nfds, readset_res, NULL, NULL, NULL);
    if (r == -1) {
        perror("select");
        exit(EXIT_FAILURE);
    }
    if (r == 0) {
        fprintf(stderr, "select: timeout\n");
        exit(EXIT_FAILURE);
    }
}
