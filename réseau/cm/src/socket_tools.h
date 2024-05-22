#ifndef _SOCKET_TOOLS_H
#define _SOCKET_TOOLS_H

#include <arpa/inet.h>

void build_inet_address(struct sockaddr_in* addr, const char* host, const char* port);
void display_inet_address(int s /* socket fd's*/);

#endif /* _SOCKET_TOOLS_H */
