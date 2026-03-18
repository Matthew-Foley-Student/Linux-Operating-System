#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5
#define PROD_COUNT 10

// Structure to define the shared memory layout
struct CircularBuffer {
    int buffer[BUFFER_SIZE];
    int head; // Producer writes here
    int tail; // Consumer reads here
    int count; // Number of items in buffer
};

// Semaphore operations
void sem_op(int semid, int op) {
    struct sembuf sb = {0, op, 0};
    semop(semid, &sb, 1);
}

void main() {
    int shmid, sem_mutex, sem_empty, sem_full;
    struct CircularBuffer *cb;
	
	long long Stop =0;

    // 1. Create Shared Memory
    shmid = shmget(IPC_PRIVATE, sizeof(struct CircularBuffer), IPC_CREAT | 0666);
    cb = (struct CircularBuffer *)shmat(shmid, NULL, 0);

    // Initialize Buffer
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;

    // 2. Create Semaphores
    sem_mutex = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    sem_empty = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    sem_full = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

    semctl(sem_mutex, 0, SETVAL, 1); // Binary semaphore
    semctl(sem_empty, 0, SETVAL, BUFFER_SIZE); // Initially all empty
    semctl(sem_full, 0, SETVAL, 0); // Initially none full
 while (Stop != 5){
    pid_t pid = fork();

    if (pid == 0) {
        // --- Consumer (Child Process) ---
        for (int i = 0; i < PROD_COUNT; i++) {
            sem_op(sem_full, -1); // Wait for full slot
            sem_op(sem_mutex, -1); // Lock

            int item = cb->buffer[cb->tail];
            cb->tail = (cb->tail + 1) % BUFFER_SIZE;
            printf("Consumer consumed: %d\n", item);
            fflush(stdout);

            sem_op(sem_mutex, 1); // Unlock
            sem_op(sem_empty, 1); // Signal empty slot
            sleep(1); // Simulate consumption time
			
        }
        exit(0);
    } else if (pid > 0) {
        // --- Producer (Parent Process) ---
        for (int i = 0; i < PROD_COUNT; i++) {
            sem_op(sem_empty, -1); // Wait for empty slot
            sem_op(sem_mutex, -1); // Lock

            cb->buffer[cb->head] = i;
            printf("Producer produced: %d at %d\n", i, cb->head);
            fflush(stdout);
            cb->head = (cb->head + 1) % BUFFER_SIZE;

            sem_op(sem_mutex, 1); // Unlock
            sem_op(sem_full, 1); // Signal full slot
            sleep(0); // Faster production
			Stop = (rand() % ( 5 - 0  + 1)) + 0;
        }

        wait(NULL); // Wait for consumer

        // Cleanup
        shmdt(cb);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(sem_mutex, 0, IPC_RMID);
        semctl(sem_empty, 0, IPC_RMID);
        semctl(sem_full, 0, IPC_RMID);
    }
}

}
