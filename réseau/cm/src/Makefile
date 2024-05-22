CC = gcc
CFLAGS = -Wall --std=c99 -pthread -D_XOPEN_SOURCE=700
LDFLAGS = -pthread
LIBS =

PRG = create_udp_socket sockaddr_un bind \
      main_client_socket_unix_DGRAM main_client_socket_unix_DGRAM2 main_server_socket_unix_DGRAM   \
      main_client_socket_unix_STREAM main_server_socket_unix_STREAM \
      main_server_socket_TCP main_client_socket_TCP \
      main_server_socket_TCP_threads \
      main_server_socket_UDP main_client_socket_UDP \
      convert_inet_addr \
      dns_lookup \
      serv_by_name \
      inet_pton \
      listen sockaddr_in \
      getaddrinfo getaddrinfo2 getnameinfo \
      getsockname server_select socket_timeout

all: $(PRG) java_dir ipv6_dir

java_dir:
	cd Java && $(MAKE) all

ipv6_dir:
	cd IPv6 && $(MAKE) all

main_client_socket_unix_DGRAM.o: main_client_socket_unix_DGRAM.c adresse_serveur_unix.h

main_client_socket_unix_DGRAM2.o: main_client_socket_unix_DGRAM2.c adresse_serveur_unix.h

main_server_socket_unix_DGRAM.o: main_server_socket_unix_DGRAM.c  adresse_serveur_unix.h

main_server_socket_TCP: main_server_socket_TCP.o socket_tools.o

main_server_socket_TCP.o: main_server_socket_TCP.c socket_tools.h

main_client_socket_TCP: main_client_socket_TCP.o socket_tools.o

main_client_socket_TCP.o: main_client_socket_TCP.c socket_tools.h

main_server_socket_TCP_threads: main_server_socket_TCP_threads.o socket_tools.o

main_server_socket_TCP_threads.o: main_server_socket_TCP_threads.c socket_tools.h

socket_tools.o: socket_tools.c socket_tools.h

clean:
	$(RM) *.o $(PRG) *~
	cd Java && $(MAKE) clean
	cd IPv6 && $(MAKE) clean
