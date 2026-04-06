#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

#define MAX_PROG_LEN 10 // Max terms in a "program"
#define MAX_PROCS 10 // Max number of processes
#define QUANTUM 40 // Time quantum, ms

#define MIN(x,y) ((x)<(y)?(x):(y)) // Compute the minimum

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

int all_exited(int argc) {
    for(int i=0; i < argc-1; i++) {
        if(table[i].state != exited) {
            return 0;
        }
    }
    return 1;
}

void decrement_sleep_time(int time, int argc) {
    for(int i=0; i < argc-1; i++) {
        if(table[i].state == sleeping) {
            table[i].sleep_time_remaining -= time;
        }
    }
}

int next_available_process(int argc) {
    int soonest_pid = 0;
    for (int i=0; i < argc-1; i++) {
        if (table[i].state == sleeping) {
            soonest_pid = i;
            break;
        }
    }

    for (int i=0; i < argc-1; i++) {
        if (table[i].state == sleeping) {
            if(table[i].sleep_time_remaining < table[soonest_pid].sleep_time_remaining) {
                soonest_pid = i;
            }
        }
    }
    return soonest_pid;
}




/**
 * Main.
 */
int main(int argc, char **argv)
{
    int clock = 0;

    struct queue *q = queue_new();

    for(int i=1; i < argc; i++) {

        char *token;
        int counter = 0;
        if ((token = strtok(argv[i], ",")) != NULL) do {
            table[i-1].instructions[counter] = atoi(token);
            counter++;
        } while ((token = strtok(NULL, ",")) != NULL);

        table[i-1].pid = i - 1;
        table[i-1].state = ready;
        table[i-1].sleep_time_remaining = 0;
        table[i-1].awake_time_remaining = table[i-1].instructions[0];
        table[i-1].pc = 0;

    }

    for (int i=0; i < argc-1; i++) {
        queue_enqueue(q, &table[i]);
    }

    while (all_exited(argc) != 1) {
        if(queue_is_empty(q)) {
            int next_process = next_available_process(argc);
            clock += table[next_process].sleep_time_remaining;
            decrement_sleep_time(table[next_process].sleep_time_remaining, argc);
        }

        printf("=== Clock %d ms ===\n", clock);

        //Find sleeping processes with < 0 sleep time
        for(int i=0; i < argc-1; i++){
            if(table[i].state == sleeping) {
                if(table[i].sleep_time_remaining <= 0){
                    table[i].pc ++;
                    if(table[i].instructions[table[i].pc] == 0){
                        table[i].state = exited;
                        printf("PID %d: Exiting\n", i);
                    }
                    else{
                        table[i].state = ready;
                        table[i].awake_time_remaining = table[i].instructions[table[i].pc];
                        printf("PID %d: Waking up for %d ms\n", i, table[i].instructions[table[i].pc]);
                        queue_enqueue(q, &table[i]);
                    }
                }
            }
        }

        struct process* p = queue_dequeue(q);
        if (p == NULL) continue;

        printf("PID %d: Running\n", p->pid);

        //Run process
        int lower_time = MIN(p->awake_time_remaining, QUANTUM);
    
        p->awake_time_remaining -= lower_time;
        decrement_sleep_time(lower_time, argc);

        if (p->awake_time_remaining > 0){
            queue_enqueue(q, p);
        }
        else{
            p->pc ++;
            if(p->instructions[p->pc] == 0){
                p->state = exited;
                printf("PID %d: Exiting\n", p->pid);
            }
            else{
                p->state = sleeping;
                p->sleep_time_remaining = p-> instructions[p->pc];
                printf("PID %d: Sleeping for %d ms\n", p->pid, p->sleep_time_remaining);
            }
        }

        clock += lower_time;

        printf("PID %d: Ran for %d ms\n", p->pid, lower_time);

    }

    queue_free(q);
}
