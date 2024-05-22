#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

int print_sockname(int sock);
int print_sockaddr(const struct sockaddr *sa);

int main(void) {
    int s;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    printf("Essai sur une socket non liée à une adresse : ");
    print_sockname(s);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    
    struct addrinfo *res;
    int errnum;
    if ((errnum = getaddrinfo(NULL, "8080", &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(errnum));
        return EXIT_FAILURE;
    }
    if (res == NULL) {
        fprintf(stderr, "getaddrinfo: pas d'adresse de trouvée\n");
        return EXIT_FAILURE;
    }
    
    if (bind(s, res->ai_addr, res->ai_addrlen) != 0) {
        perror("bind");
         return EXIT_FAILURE;
    }
    freeaddrinfo(res);

    printf("Essai sur une socket liée à la boucle locale : ");
    print_sockname(s);

    return EXIT_SUCCESS;
}

int print_sockname(int sock) {
    struct sockaddr_storage ss;
    socklen_t sl = sizeof(struct sockaddr_storage);
    if (getsockname(sock, (struct sockaddr *) &ss, &sl) == -1) {
        perror("getsockname");
        return -1;
    }

    if (sl > sizeof(struct sockaddr_storage)) {
        fprintf(stderr, "getsockname: buffer too small\n");
    }
    
    return print_sockaddr((const struct sockaddr *) &ss);
}

int print_sockaddr(const struct sockaddr *sa) {
    char ip[40];
    const char *r;
    switch (sa->sa_family) {
    case AF_INET:
    {
        struct sockaddr_in *sin = (struct sockaddr_in *) sa;
        r = inet_ntop(sin->sin_family, (const void *) &sin->sin_addr, ip, sizeof ip);
        break;
    }
    case AF_INET6:
    {
        struct sockaddr_in6 *sin = (struct sockaddr_in6 *) sa;
        r = inet_ntop(sin->sin6_family, (const void *) &sin->sin6_addr, ip, sizeof ip);
        break;
    }
    default:
        fprintf(stderr, "inet_ntop: type d'adresse non supporté\n");
        return -1;
    }

    if (r == NULL) {
        perror("inet_ntop");
        return EXIT_FAILURE;
    }
    
    printf("%s\n", ip);

    return 0;
}

