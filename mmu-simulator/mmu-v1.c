#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pthread.h>
//#include <semaphore.h>

#define MAIN 64  /* Size of main memory in KB */
#define VIRT 1024  /* Size of virtual memory in KB */

#define FRAM 8  /* Size of frames in KB */
#define PAGE 8  /* Size of pages in KB */

#define P_NUM 2

typedef struct main {
    int frames[MAIN / FRAM];
    pthread_mutex_t lock;
} Main;  /*  */

typedef struct _virtual {
    int pages[VIRT / PAGE];
} Virtual;

int curr_frame = 0;

void* access_main_memory(Main main_memory) {
    pthread_mutex_lock(&main_memory.lock);
    sleep(2);
    printf("[%d]\n", getpid());
    pthread_mutex_unlock(&main_memory.lock);
}

int main(void) {
    Main main_memory;
    pthread_mutex_init(&main_memory.lock, NULL);

    short int processes_pid[P_NUM];
    short int i = 0;

    for (i = 0; i < P_NUM; i++) {
        processes_pid[i] = fork();
        if (processes_pid[i] == -1) {
            printf("Error creating process\n");
            return 1;
        }
        if (processes_pid[i] == 0) {  /* process - child process */
            while(1) {
                
            }
        }
    }

    sleep(10);

    for (i = 0; i < P_NUM; i++) {
        printf("%d ", processes_pid[i]);
        kill(processes_pid[i], SIGKILL);
    }
    pthread_mutex_destroy(&main_memory.lock);
    return 0;
}