holdalldir = ../holdall/
CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings -O2 -I$(holdalldir)
LDFLAGS =
VPATH = $(holdalldir)
objects = hashtable.o $(holdalldir)holdall.o main.o
executable = main

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

hashtable.o: hashtable.c hashtable.h
holdall.o: holdall.c holdall.h
main.o: main.c hashtable.h holdall.h
