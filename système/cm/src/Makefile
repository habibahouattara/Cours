#CC = clang
JAVAC = javac

CPPFLAGS = -D_XOPEN_SOURCE -D_FORTIFY_SOURCE=2

CFLAGS = -std=c18 -Wpedantic \
         -Wall -Wextra -Wconversion -Wwrite-strings -Wstrict-prototypes -Werror \
         -fstack-protector-all -fpie \
         -O2 -g

LDFLAGS = -Wl,-z,relro,-z,now -pie

PRGS = syscall return \
       pid pgid sid sid2 setuid uid groups nice fork wait execve environ newsession open read stat \
       questionfd dup dup2 readdir readlink chmod semnomme semanonyme shm \
       ftruncate mmap filedemessages pipe pipebuf sigpipe fifo \
       sigaction readwithsignal readwithsarestart sigsuspend alarm sigchild nocldwait \
       sigrtmin sigaction_temps_reel sigqueue \
       pthread_create pthread_bonjour pthread_exit pthread_join pthread_attr_init pthread_detach pthread_create_with_args \
       thread_unsafe pthread_mutex pthread_cond pthread_rwlock pthread_wrong \
       strerror_r \
       test_commutation test_commutation2 test_commutation3 \
       fork_env \
       producteurconsommateur ProducteurConsommateur.class

all: $(PRGS)

semnomme: semnomme.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -pthread -o $@

semnomme.o: semnomme.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

semanonyme: semanonyme.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -pthread -lrt -o $@

semanonyme.o: semanonyme.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

shm: shm.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -lrt -o $@

filedemessages: filedemessages.o console.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -lrt -o $@

filedemessages.o: filedemessages.c console.h

console.o: console.c console.h

producteurconsommateur: producteurconsommateur.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -lrt -o $@ $(LDLIBS) 

pthread_create: pthread_create.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_create.o: pthread_create.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

pthread_bonjour: pthread_bonjour.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_bonjour.o: pthread_bonjour.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

pthread_exit: pthread_exit.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_exit.o: pthread_exit.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

pthread_attr_init: pthread_attr_init.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_attr_init.o: pthread_attr_init.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

pthread_join: pthread_join.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_join.o: pthread_join.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

pthread_detach: pthread_detach.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_detach.o: pthread_detach.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

pthread_create_with_args: pthread_create_with_args.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_create_with_args.o: pthread_create_with_args.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

thread_unsafe: thread_unsafe.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

thread_unsafe.o: thread_unsafe.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

pthread_mutex: pthread_mutex.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_mutex.o: pthread_mutex.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

pthread_cond: pthread_cond.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_cond.o: pthread_cond.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

pthread_rwlock: pthread_rwlock.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_rwlock.o: pthread_rwlock.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

pthread_wrong: pthread_wrong.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

pthread_wrong.o: pthread_wrong.c
	$(CC) -c -D_XOPEN_SOURCE -D_FORTIFY_SOURCE -std=c18 -Wpedantic -Wall -Wextra -Wconversion -Wwrite-strings -Werror -fstack-protector-all -fpie -O -g -pthread $<

strerror_r: strerror_r.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) -pthread -o $@ $(LDLIBS) 

strerror_r.o: strerror_r.c
	$(COMPILE.c) -pthread $(OUTPUT_OPTION) $<

ProducteurConsommateur.class: ProducteurConsommateur.java
	$(JAVAC) ProducteurConsommateur.java

clean:
	$(RM) *.o *~ *.class out.txt $(PRGS) \#*\# a.out foo* fake* test.bin

