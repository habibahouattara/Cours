hashtabledir = ../hashtable/
holdalldir = ../holdall/
listsdir = ../lists/
CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -I$(hashtabledir) -I$(holdalldir) -I$(listsdir) -O2
LDFLAGS =
VPATH = $(hashtabledir):$(holdalldir):$(listsdir)
objects = $(hashtabledir)hashtable.o $(holdalldir)holdall.o $(listsdir)lists.o main.o
executable = main

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

$(hashtabledir)hashtable.o: hashtable.c hashtable.h
$(holdalldir)holdall.o: holdall.c holdall.h
$(listsdir)lists.o: lists.c lists.h
main.o: main.c hashtable.h holdall.h lists.h
