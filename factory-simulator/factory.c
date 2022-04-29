/*
Nome: Luiz Alfredo Thomasini
Matricula: 1786631
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define PRODUCER_NUM 3
#define CONSUMER_NUM 2

#define PRODUCT_NUM 4
#define TIME_RANGE 6
#define QUALITY_RANGE 6


struct stat st = {0};

typedef struct {
    char name[64];
    short int manufacturing_time;
    short int quality;
} Product;

Product create_product(char **product_names, int count) {
    short int name_index = rand() % PRODUCT_NUM;
    short int manufacturing_time = rand() % TIME_RANGE;
    short int quality = rand() % QUALITY_RANGE;
    
    Product p;
    strcpy(p.name, product_names[name_index]);
    p.manufacturing_time = manufacturing_time;
    p.quality = quality;
    return p;
}

void increment_counter(int pid) {
    FILE *fp;
    char buff[255];
    char filename[64];
    
    snprintf(filename, sizeof(filename), "/factory_log/%d.txt", pid);
    printf(filename);

    fp = fopen(filename, "r");
    fgets(buff, 255, (FILE*)fp);
    printf(buff);
    fclose(fp);
}


int main(int argc, char* argv[]) {
    short int producer_pids[PRODUCER_NUM]; 
    short int consumer_pids[CONSUMER_NUM];
    short int i;
    int fd[2];

    char *product_names[PRODUCT_NUM+1] = {"Notebook", "Tablet", "Smartphone", "Smartwatch"};
    char product_qualities[QUALITY_RANGE+1][15] = {"Garbage", "Poor", "Medium", "Good", "Excelent", "a Masterpiece!"};

    // Code start
    printf("\nTurning on the conveyor belt...\n");
    sleep(5);
    if (pipe(fd) == -1) {
        printf("Error creating pipe\n");
        return 1;
    }
    if (stat("/factory_log", &st) == -1) {
        mkdir("/factory_log", 0700);
    }
    

    printf("Cleaning up consumers...\n");
    sleep(5);
    for (i = 0; i < CONSUMER_NUM; i++) {
        consumer_pids[i] = fork();
        if (consumer_pids[i] == -1) {
            printf("Error creating consumer process\n");
            return 2;
        }
        if (consumer_pids[i] == 0) {  // consumer - child process
            close(fd[1]);

            Product product;
            char quality[15];
            while (1) {
                read(fd[0], &product, sizeof(product));
                strcpy(quality, product_qualities[product.quality]);

                printf("[%d] Finished consuming '%s' and it is '%s'.\n", getpid(), product.name, quality);
            }
        }
    }

    printf("Gearing up producers...\n\n");
    sleep(5);
    for (i = 0; i < PRODUCER_NUM; i++) {
        producer_pids[i] = fork();
        if (producer_pids[i] == -1) {
            printf("Error creating producer process\n");
            return 3;
        }
        if (producer_pids[i] == 0) {  // producer - child process
            close(fd[0]);
            
            srand(getpid());
            Product product;
            while (1) {
                product = create_product(product_names, PRODUCT_NUM);
                sleep(product.manufacturing_time);
                
                printf("[%d] Manufactoring '%s' for %d seconds.\n", getpid(), product.name, product.manufacturing_time);
                write(fd[1], &product, sizeof(product));
            }
        }
    }
    sleep(TIME_RANGE * 5);

    for (i = 0; i < PRODUCER_NUM; i++) {
        printf("%d ", producer_pids[i]);
        kill(producer_pids[i], SIGKILL);
    }
    printf("- Shutting down producers...\n");

    sleep(TIME_RANGE);

    for (i = 0; i < CONSUMER_NUM; i++) {
        printf("%d ", consumer_pids[i]);
        kill(consumer_pids[i], SIGKILL);
    }
    printf("- Sending consumers home...\n");
    
    wait(NULL);
    return 0;
}