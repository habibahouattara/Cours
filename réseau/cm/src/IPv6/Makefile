CC = gcc
CFLAGS = --std=c11 -Wall -Wextra -Werror -pedantic -D_XOPEN_SOURCE=600 -pthread
LDFLAGS = -pthread

PRG = getaddrinfo main_client_socket_TCP main_server_socket_TCP_threads 

all: $(PRG)

getaddrinfo: getaddrinfo.o

getaddrinfo.o: getaddrinfo.c

main_client_socket_TCP: main_client_socket_TCP.o socket_tools.o

main_server_socket_TCP_threads: main_server_socket_TCP_threads.o socket_tools.o

main_client_socket_TCP.o: main_client_socket_TCP.c socket_tools.h

main_server_socket_TCP_threads.o: main_server_socket_TCP_threads.c socket_tools.h

socket_tools.o: socket_tools.c socket_tools.h

clean:
	$(RM) *.o $(PRG) *~
