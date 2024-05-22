assumedir = ../../5/assume/
CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings -O2 -I$(assumedir) -DASSUME_EXPR_EXPANSION -DSETMIN
LDFLAGS =
VPATH = $(assumedir)
objects0 = $(assumedir)assume.o main0.o set_mp.o
executable0 = main0
objects1 = $(assumedir)assume.o main1.o set_mp.o
executable1 = main1
objects2 = $(assumedir)assume.o main2.o set_mp.o
executable2 = main2
executables = main0 main1 main2

all: $(executables)

clean:
	$(RM) $(objects0) $(executable0)
	$(RM) $(objects1) $(executable1)
	$(RM) $(objects2) $(executable2)

$(executable0): $(objects0)
	$(CC) $(objects0) $(LDFLAGS) -o $(executable0)
$(executable1): $(objects1)
	$(CC) $(objects1) $(LDFLAGS) -o $(executable1)
$(executable2): $(objects2)
	$(CC) $(objects2) $(LDFLAGS) -o $(executable2)

assume.o: assume.c assume.h
main0.o: main0.c set_mp.h
main1.o: main1.c set_mp.h
main2.o: main2.c set_mp.h
set_mp.o: set_mp.c set_mp.h assume.h
