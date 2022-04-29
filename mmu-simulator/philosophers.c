/********************************************************************
 * Sistemas Operacionais                                            *
 * Profa. Andriele Busatto do Carmo                                 *
 *                                                                  *
 * Enunciado:                                                       *
 * Há cinco filósofos em torno de uma mesa. Um garfo é colocado     *
 * entre cada filósofo. Cada filósofo, alternadamente, deve         *
 * pensar e comer. Para comer, um filósofo deve utilizar dois       *
 * garfos (da esquerda e da direita, imediatamente). Um garfo       *
 * só pode ser pego se não estiver em uso por outro filósofo.       *
 * Após comer, o filósofo deve liberar os garfos que utilizou.      *
 * Um filósofo pode segurar o grafo da direita ou o garfo da        *
 * esquerda assim que estiverem disponíveis, mas só pode            *
 * começar a comer quando os dois estiverem sob sua posse.          *
 *                                                                  * 
 * Implementação a partir do algoritmo do livro: Operating Systems: *
 * Design and Implementation.                                       *
 * Autores: Andrew S. Tanenbaum e Albert S. Woodhull.               *
 * ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5 /* The amount of philosophers */

typedef enum {
    THINKING, /* A philosopher is thinking */
    HUNGRY,   /* A philosopher is trying to get chopsticks */
    EATING    /* A philosopher is eating */
} State;

typedef struct {
    State     state;           /* Current state of this philosopher */
    int       index;           /* Position of philosopher in the array/table */
    pthread_t thread;          /* Thread object for this philosopher */
    sem_t     took_chopsticks; /* Semaphore used to wake up this philosopher */
} Philosopher;

Philosopher philosophers[N];
sem_t mutex; /* Mutual exclusion for critical sections */

int counter = 1;
int random_time;

/* Neighbor to the left of i */
int left_of(int const i) {
    return (i + N - 1) % N;
}

/* Neighbor to the right of i*/
int right_of(int const i) {
    return (i + 1) % N;
}

/* This function verifies if it is possible to a philosopher take chopsticks 
 * in order to eat */
void verify_availability(int const p) {
    int const left  = left_of(p),
              right = right_of(p);

    if (   philosophers[p    ].state == HUNGRY
        && philosophers[left ].state != EATING
        && philosophers[right].state != EATING) {

        philosophers[p].state = EATING;
        sem_post(&philosophers[p].took_chopsticks);
    }
}

/* This function represents the actions of thinking */
void think(int const p) {
    printf("Philosopher %d is thinking.\n", p);
    fflush(stdout);
    random_time = (int) (0.001 * random());
    usleep(random_time);
}

/* This function represents the action of eating */
void eat(int const p) {
    printf("Philosopher %d is eating.\n", p);
    fflush(stdout);
    random_time = (int) (0.001 * random());
    usleep(random_time);
}

/* This function allows philosophers to take chopsticks */
void take_chopsticks(int const p) {
    sem_wait(&mutex);

    philosophers[p].state = HUNGRY;
    verify_availability(p);

    sem_post(&mutex);
    sem_wait(&philosophers[p].took_chopsticks);

    printf("Philosopher %d took chopsticks.\n", p);
    fflush(stdout);
}

/* This function allows philosophers to return chopsticks */
void return_chopsticks(int const p) {
    sem_wait(&mutex);

    philosophers[p].state = THINKING;

    printf("Philosopher %d returned chopsticks.\n", p);
    fflush(stdout);

    verify_availability(left_of(p));  /* Verifies if the philosopher to the left can eat  */
    verify_availability(right_of(p)); /* Verifies if the philosopher to the right can eat */
    counter++;
    
    sem_post(&mutex);
}

/* This function represents all actions performed by philosophers */
void* philosopher(void* philo) {
    int const p = *((int*) philo);

    while (1) {
        think(p);
        take_chopsticks(p);
        eat(p);
        return_chopsticks(p);

        if(counter > 10)
            break;
    }

    pthread_exit(0);
}

int main(void) {
    int i;

    sem_init(&mutex, 0, 1);

    for (i = 0; i < N; ++i) {
        philosophers[i].index = i;
        philosophers[i].state = THINKING;

        sem_init(&philosophers[i].took_chopsticks, 0, 0);
        pthread_create(&philosophers[i].thread, NULL, philosopher, &philosophers[i].index);
    }

    for(i = 0; i < N; ++i)
        pthread_join(philosophers[i].thread, NULL);

    for(i = 0; i < N; ++i)
        sem_destroy(&philosophers[i].took_chopsticks);

    return 0;
}

