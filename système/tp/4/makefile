CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic
LDFLAGS = -lm
RM = rm

ccp: ccp.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
pwd: pwd.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
*.o: *.c
	$(CC) -c $< $(CFLAGS)
	
clean:
	$(RM) -f *.o

