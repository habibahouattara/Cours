CC = gcc

CPPFLAGS = -D_XOPEN_SOURCE=500 -D_FORTIFY_SOURCE=2

CFLAGS = -std=c18 -Wpedantic -Wall -Wextra -Wconversion -Wwrite-strings -Werror \
					-Wstrict-prototypes -fstack-protector-all -fpie -O2 -g

LDFLAGS = -lm

LDLIBS = -lrt

RM = rm

PRGS = client demon tube foo.txt

client: client.o
	$(CC) $< $(LDFLAGS) -pthread -o $@ $(LDLIBS)

demon: demon.o
	$(CC) $< $(LDFLAGS) -pthread -o $@ $(LDLIBS)
	
*.o: *.c
	$(CC) -c $^ $(CPPFLAGS) $(CFLAGS) -o $@

clean:
	$(RM) -f *.o $(PRGS)
