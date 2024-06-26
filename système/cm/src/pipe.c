#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_INT 4096

int main(void) {
    int tube[2];
    if (pipe(tube) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    ssize_t n;
    int i;
    switch (fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        if (close(tube[0]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < NUM_INT; ++i) {
            printf("Écriture de %d\n", i);
            if (write(tube[1], &i, sizeof(i)) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        if (close(tube[1]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
    }
        break;
    default:  
        if (close(tube[1]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        while ((n = read(tube[0], &i, sizeof(i))) > 0) {
            printf("Lecture de %d\n", i);
        }
        if (n == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (close(tube[0]) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
        break;
    }
    exit(EXIT_SUCCESS);
}
