CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic
LDFLAGS = -lm
RM = rm

ex1: ex1.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
ex2: ex2.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
*.o: *.c
	$(CC) -c $< $(CFLAGS)
	
clean:
	$(RM) -f *.o
