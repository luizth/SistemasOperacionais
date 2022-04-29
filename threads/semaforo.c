#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

char buffer;
sem_t s0, s1; // Declaração das variáveis

void *produtor() {
	char  item;
	while(1) {
        sem_wait (&s0);
		item = 'A';
		printf("Produzi A\n");
		buffer = item;
        sem_post(&s1);
	}

    pthread_exit(0);
}

void *consumidor() {
	char item;
	while(1) {
		sem_wait(&s1);
		item = buffer;
		printf(">>>>>>>>>>Consumi o item %c\n", item);
		sem_post(&s0);
	}

    pthread_exit(0);
}

int main() {
	pthread_t t1, t2;
	sem_init (&s0, 0, 1);
	sem_init (&s1, 0, 0);
	(void) pthread_create(&t1, NULL, produtor, NULL);
	(void) pthread_create(&t2, NULL, consumidor, NULL);
   	(void) pthread_join(t1, NULL);
	(void) pthread_join(t2, NULL);

    sem_destroy(&s0);
    sem_destroy(&s1);

    return 0;
}

