CC = gcc
CPPFLAGS = -D_XOPEN_SOURCE=500 -D_FORTIFY_SOURCE=2
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic
LDFLAGS = -lm
RM = rm

ex1: ex1.o
	$(CC) -o $@ $^ $(CPPFLAGS) $(LDFLAGS)
	
*.o: *.c
	$(CC) -c $< $(CFLAGS)
	
clean:
	$(RM) -f *.o

