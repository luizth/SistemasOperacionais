#include <pthread.h>
#include <stdio.h>

char buffer;
int count = 0;
pthread_mutex_t mutex;

void *produtor() {
	char  item;
	while(1) {
		pthread_mutex_lock(&mutex);
		item = 'A';
		printf("Produzi  A\n");
		buffer = item;
		count = 1;
		pthread_mutex_unlock(&mutex);
	}
}

void *consumidor() {
	char item;
	while(1) {
		pthread_mutex_lock(&mutex);	
		if (count == 1) {
			item = buffer;
			printf(">>>>>>>>>>>>>>>>Consumi o item %c\n", item);
			count = 0;
		}
		pthread_mutex_unlock(&mutex);
	}
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, NULL);

    (void) pthread_create(&t1, NULL, produtor, NULL);
    (void) pthread_create(&t2, NULL, consumidor, NULL);
   
    (void) pthread_join(t1, NULL);
    (void) pthread_join(t2, NULL);
    return 0;
}

