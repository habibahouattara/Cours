holdalldir = ../holdall/
CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings -O2 -I$(holdalldir)
LDFLAGS =
VPATH = $(holdalldir)
objects = table.o $(holdalldir)holdall.o main.o
executable = main

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

table.o: table.c table.h
holdall.o: holdall.c holdall.h
main.o: main.c table.h holdall.h
