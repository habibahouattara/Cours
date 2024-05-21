CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic
LDFLAGS = -lm
RM = rm
PRG = client server

all:$(PRG)

server: server.c
	$(CC) -o $@ $^ $(LDFLAGS)
	
client: client.c
	$(CC) -o $@ $^ $(LDFLAGS)
	
*.o: *.c
	$(CC) -c $< $(CFLAGS)
	
clean:
	$(RM) $(PRG) -f *.o
