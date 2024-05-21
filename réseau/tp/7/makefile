CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic
LDFLAGS = -lm
RM = rm
PRG = prog

all:$(PRG)

prog: prog.c
	$(CC) -o $@ $^ $(LDFLAGS)

*.o: *.c
	$(CC) -c $< $(CFLAGS)
	
clean:
	$(RM) $(PRG) -f *.o
