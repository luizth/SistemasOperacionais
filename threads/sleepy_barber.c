/********************************************************************
 * Análise e Aplicação de Sistemas Operacionais                     *
 * Luiz Alfredo Thomasini                                           *
 *                                                                  *
 * Problema do barbeiro sonolento:                                  *                   
 * A analogia do problema usa como cenario uma barbearia.           *
 *                                                                  *
 * A barbearia possui:                                              *
 * - a cadeira do barbeiro                                          *
 * - e uma sala de espera para os clientes com um numero limitado   *
 *   de lugares.                                                    *
 *                                                                  *
 * Ao finalizar o corte de cabelo de um cliente, o barbeiro:        *
 * - libera o cliente e vai até a sala de espera para ver se tem    *
 *   algum cliente esperando.                                       *
 *                                                                  *
 * - Se há cliente esperando, traz um deles para cortar o cabelo.   *
 * - Se não há clientes esperando, o barbeiro volta para a cadeira  *
 *   de corte para dormir.                                          *
 *                                                                  *
 * Quando um cliente chega à barbearia: o cliente olha o que o      *
 * barbeiro está fazendo.                                           *
 * - Se o barbeiro estiver dormindo, ele o acorda e senta na        *
 *   cadeira.                                                       *
 * - Se o barbeiro estiver trabalhando, o cliente volta para a sala *
 *   de espera.                                                     *
 * - Se houver cadeira disponível na sala de espera, o cliente      * 
 *   senta e aguarda a sua vez.                                     *
 * - Se não houver cadeira disponível na sala de espera, o cliente  *
 *   vai embora.                                                    *
 * ******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5 // Size of the waiting room

/* Buffer control for the waiting room */
sem_t sem_empty; 
sem_t sem_full;

pthread_mutex_t barber_chair;

int waiting_room[N];
int client_count = 0;

/* This function simulates the barber's work */
void* work(void* args) {
    while(1) {
        // Barber is waiting for a client to arrive at the waiting room.
        sem_wait(&sem_full);
        pthread_mutex_lock(&barber_chair);  // Barber occupies the barber chair.

        // Barber pulls the last client from the waiting room.
        int c = waiting_room[client_count - 1];
        client_count--;
        
        pthread_mutex_unlock(&barber_chair);
        sem_post(&sem_empty); // Barber allows another client to enter the barbershop.

        printf("Barber started cutting hair of client %d.\n", c);
        sleep(rand() % 10);
    }
}

/* This function simulates clients behavior */
void* arrive(void* args) {
    while(1) {
        /* At X times, a new client arrives at the barbershop. */
        int c = rand() % 100; // random id for the client
        sleep(rand() % 10);
        printf("Client %d arrived at the waiting room.\n", c);

        /* The client waits for some room to enter the barbershop. */
        sem_wait(&sem_empty); 
        pthread_mutex_lock(&barber_chair); // The waits for the barber to free the barber chair

        waiting_room[client_count] = c;
        client_count++;

        pthread_mutex_unlock(&barber_chair); // The client frees the barber chair
        sem_post(&sem_full);
    }
}

int main(void) {
    pthread_t barber;
    pthread_t clients;

    sem_init(&sem_empty, 0, N);
    sem_init(&sem_full, 0, 0);

    if (pthread_create(&barber, NULL, &work, NULL) != 0) {
        perror("Failed to create thread");
    }
    if (pthread_create(&clients, NULL, &arrive, NULL) != 0) {
        perror("Failed to create thread");
    }
    if (pthread_join(barber, NULL) != 0) {
        perror("Failed to join thread");
    }
    if (pthread_join(clients, NULL) != 0) {
        perror("Failed to join thread");
    }

    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    return 0;
}
