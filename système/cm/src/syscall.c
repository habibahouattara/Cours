#define _GNU_SOURCE         // Pour syscall
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include <errno.h>

int main(void) {
    const char * texte = "Bonjour\n";

    // SYS_write contient le numéro de l'appel système write,
    // sur ma machine 64 bits, il s'agit du numéro 1
    // (sur des processeurs 32 bits de la famille des x86,
    // il s'agit du numéro 4)
    syscall(SYS_write, STDOUT_FILENO, texte, strlen(texte));
    
    // gettid est un appel système Linux pour lequel il n'est pas
    // fournit de fonction enveloppe dans la glibc
    long tid = syscall(SYS_gettid);
    printf("TID : %ld\n", tid);

    return EXIT_SUCCESS;
}
