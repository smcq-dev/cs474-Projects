#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

#define MAX_THREADS 20

// TODO Make global lock variable(s) here.
atomic_int next_ticket;
atomic_int now_serving;

/**
 * Acquire the lock.
 *
 * `tid` is the thread ID number. You only need this to print out the
 * required output.
 */
void lock(int tid) {
    // TODO
    int next_ticket_cpy = atomic_fetch_add(&next_ticket, 1);

    // Print out the number of the ticket that was just grabbed:
    //printf("Thread %d: Grabbed ticket %d\n", tid, /* ... */);

    // TODO
    printf("Thread %d: Grabbed ticket %d\n", tid, next_ticket_cpy);
    while(now_serving != next_ticket_cpy);
}

/**
 * Unlock the lock.
 */
void unlock(void) {
    // TODO
    atomic_fetch_add(&now_serving, 1);

}

/**
 * Run function for all threads.
 */
void *run(void *args)
{
    int id = *(int*)args;

    printf("Thread %d: Locking\n", id);
    lock(id);

    printf("Thread %d: Running\n", id);
    usleep(50000); // 50 ms

    printf("Thread %d: Unlocking\n", id);
    unlock();

    return NULL;
}

/**
 * Main.
 */
int main(int argc, char *argv[])
{
    pthread_t t[MAX_THREADS];
    int id[MAX_THREADS];

    if (argc != 2) {
        fprintf(stderr, "usage: ticketlock threadcount\n");
        return 1;
    }

    int thread_count = atoi(argv[1]);

    if (thread_count < 1 || thread_count > MAX_THREADS) {
        fprintf(stderr, "ticketlock: thread count must be betweeen 1 and %d\n",
            MAX_THREADS);
        return 2;
    }

    for (int i = 0; i < thread_count; i++) {
        id[i] = i;
        pthread_create(t + i, NULL, run, id + i);
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(t[i], NULL);
    }
}