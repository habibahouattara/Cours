assumedir = ../../5/assume/
CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings -O2 -I$(assumedir) -DASSUME_EXPR_EXPANSION -DSETMIN
LDFLAGS =
VPATH = $(assumedir)
objects = $(assumedir)assume.o main.o set_m_int.o
executable = main

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

assume.o: assume.c assume.h
main.o: main.c set_m_int.h
set_m_int.o: set_m_int.c set_m_int.h assume.h
