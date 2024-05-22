CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic
LDFLAGS = -lm
RM = rm

ex2_1: ex2_1.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
ex2_2: ex2_2.o
	$(CC) -o $@ $^ $(LDFLAGS)

ex2_3: ex2_3.o
	$(CC) -o $@ $^ $(LDFLAGS)
	
*.o: *.c
	$(CC) -c $< $(CFLAGS)
	
clean:
	$(RM) -f *.o
