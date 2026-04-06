#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_PROG_LEN 10 // Max terms in a "program"
#define MAX_PROCS 10 // Max number of processes
#define QUANTUM 40 // Time quantum, ms

#define MIN(x,y) ((x)<(y)?(x):(y)) // Compute the minimum

// TODO: all the code!

enum State {
    ready,
    sleeping,
    exited
};

struct process {
    int pid;
    enum State state;
    int sleep_time_remaining;
    int awake_time_remaining;
    int pc;
    int instructions[MAX_PROG_LEN];
    
};

struct process table[MAX_PROCS];


/**
 * Main.
 */
int main(int argc, char **argv)
{
    printf("argc: %d\n", argc);

    int clock = 0;

    struct queue *q = queue_new();

    for(int i=1; i < argc; i++) {

        char *token;
        int counter = 0;
        if ((token = strtok(argv[i], ",")) != NULL) do {
            table[i-1].instructions[counter] = atoi(token);
            counter++;
        } while ((token = strtok(NULL, ",")) != NULL);

    
    }

    for(int i=0; i < argc-1; i++){
        for(int j=0; j < MAX_PROG_LEN; j++) {
            if(table[i].instructions[j] != 0) {
                printf("%d", table[i].instructions[j]);
            }
        }
        puts("\n");
    }

    // TODO



    queue_free(q);
}
