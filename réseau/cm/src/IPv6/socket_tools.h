#ifndef _SOCKET_TOOLS_H
#define _SOCKET_TOOLS_H

#include <arpa/inet.h>

#include <netdb.h>

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

#ifndef NI__MAXSERV
#define NI_MAXSERV 32
#endif

/**
 * Construit une adresse réseau à partir d'un nom d'hôte ou d'une adresse IP.
 * Cette adresse pourra être utilisée ensuite avec une socket de type SOCK_STREAM.
 * @param addr l'adresse à remplir. En sortie contiendra une struct sockaddr_in ou
 * une struct sockaddr_in6. addr doit donc pointer sur une zone mémoire suffisemment
 * grande pour contenir ces adresses (elle peut pointer sur sockaddr_storage par exemple).
 * addr peut être NULL pour indiquer qu'on veut une adresse concernant toutes les interfaces
 * réseaux.
 * @param addr_size en sortie, contient la taille effective de addr en octets. En entrée contient la
 * taille disponible dans addr en entrée.
 * @param host l'adresse de l'hôte, ce peut être un nom de domaine, ou une adreses IPv4
 * en notation pointée ou une adresse IPv6 en écriture hexadécimale.
 * @param port le port TCP, peut être un nom de service.
 */
void build_inet_address(struct sockaddr* addr, socklen_t* addr_size, const char* host, const char* port);

void display_socket_address(int s /* socket fd's*/);

void display_inet_address(struct sockaddr* addr, socklen_t addr_size);

#endif /* _SOCKET_TOOLS_H */
