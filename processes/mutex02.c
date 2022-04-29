#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *thread_function(void *arg);
pthread_mutex_t work_mutex; // protege work_area e time_to_exit

#define WORK_SIZE 1024      // declara MUTEX e as variaveis comuns
char work_area[WORK_SIZE];  // MUTEX protege essas variaveis
int time_to_exit = 0;

int main() {
    int res;
    pthread_t a_thread;
    void *thread_result;
    
    res = pthread_mutex_init(&work_mutex, NULL); // iniciaiza MUTEX work_mutex
    res = pthread_create(&a_thread, NULL, thread_function, NULL); // cria thread a_thread
    pthread_mutex_lock(&work_mutex);
    printf("Input some text. Enter 'end' to finish\n"); // 'end' faz a outra thread ligar time_to_exit
    
    while(!time_to_exit) {
        fgets(work_area, WORK_SIZE, stdin);
        pthread_mutex_unlock(&work_mutex);
        while(1) {
            pthread_mutex_lock(&work_mutex);        // verifica se a outra thread consumiu o string lido
            if (work_area[0] != '\0') {             // testando variavel compartilhada dentro de uma
                pthread_mutex_unlock(&work_mutex);  // regiao critica
                sleep(1);
            } else {
                break;
            }
        }
    }
    pthread_mutex_unlock(&work_mutex);
    
    printf("\nWaiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result); // recebe o retorno da thread_function
    printf("Thread joind\n");                     // espera pela outra thread terminar
    pthread_mutex_destroy(&work_mutex);           // destroi o MUTEX
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) { 
    sleep(1);
    
    pthread_mutex_lock(&work_mutex);            // primeira entrada na regiao critica
    while(strncmp("end", work_area, 3) != 0) {  // strncmp compara strings
        printf("You input %lu characters\n", strlen(work_area) -1); // strlen comprimento da string
        work_area[0] = '\0';
        pthread_mutex_unlock(&work_mutex);
        sleep(1);
        pthread_mutex_lock(&work_mutex);
        while (work_area[0] == '\0')    {
            pthread_mutex_unlock(&work_mutex);
            sleep(1);
            pthread_mutex_lock(&work_mutex);
        }
    }
    time_to_exit = 1;
    work_area[0] = '\0';
    pthread_mutex_unlock(&work_mutex); // ultima saida da regiao critica
    pthread_exit(0);
}

