#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>
#include <errno.h>

int main() {
    //Create shared memory for one integer
    int *shared_var = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, 
                           MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    *shared_var = 0; // Initialize

  //Setup mutex for synchronization 
    pthread_mutex_t *mutex = mmap(NULL, sizeof(pthread_mutex_t), 
                                 PROT_READ|PROT_WRITE, 
                                 MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &attr);
    int MissedLoop =0;
    
    printf("The Program Will Now Run For The Next 60 Seconds.");

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child Process
        for(int i=0; i<60; i++) {
        sleep(1);
            pthread_mutex_lock(mutex);
            (*shared_var)++;
            printf("Child incremented: %d\n", *shared_var);
            pthread_mutex_unlock(mutex);
        }
        exit(0);
    } else {
        // Parent Process
        for(int i=0; i<20; i++) {
        sleep(3);
    int result = pthread_mutex_trylock(mutex);
    if (result == 0) {
        // The mutex was successfully locked by this thread.
        printf("Mutex was unlocked, Not Doing Anything...\n");
        // ... perform protected work ...
        pthread_mutex_unlock(mutex);
    } else if (result == EBUSY) {
        // The mutex was already locked by another thread (or potentially this one if not recursive).
        printf("Mutex is currently locked by another thread. Not blocking.\n");
        MissedLoop++;
    } else {
        // An error occurred (e.g., EINVAL for an uninitialized mutex).
        fprintf(stderr, "Error trying to lock mutex: %d\n", result);
    }
        }
        wait(NULL); // Wait for child
        printf("Final value: %d\n", *shared_var);
        printf("Final Missed Loops: %d\n",MissedLoop);
    }

    // Cleanup
    pthread_mutex_destroy(mutex);
    return 0;
}

