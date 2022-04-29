/*
* Nome: Luiz Alfredo Thomasini
* Matricula: 1786631
*
* MMU Simulator using FIFO policy for controlling memory space
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#define MAIN 64  /* Main memory size in KB */

#define FRAM 8  /* Frame size in KB */


typedef struct memory_map {
    int frame;
    int page;
    int pid;
} MemoryMap;

MemoryMap memory_map[MAIN / FRAM];
short int map_size = sizeof memory_map / sizeof memory_map[0];  /* Memory space size */
short int allocated_frames = 0;                                 /* Memory space control */

short int curr_frame = 0;  /* FIFO policy control variable */

void init_memory() {
    /* 
        Init the memory space

        return: none
    */
    for(int i = 0; i < map_size; i++) {
        memory_map[i].frame = INT_MIN;
        memory_map[i].page = INT_MIN;
        memory_map[i].pid = INT_MIN;
    }
}

int find_page_in_memory(int page) {
    /* 
        Get the frame number of the memory mapped to a page
        If page isn't in memory, returns -1
        
        return: frame ? -1
    */
    for(int i = 0; i < map_size; i++) {
        if (memory_map[i].page == page)
            return memory_map[i].frame;
    }
    return -1;
}

void manage_memory_space() {
    /*
        Controls the number of alocated frames

        return: none
    */
    if(allocated_frames != map_size) {
        allocated_frames++;
    }
}

bool memory_is_full() {
    /*
        Returns if memory is full

        return: true ? false
    */
    return allocated_frames == map_size;
}

void manage_queue() {
    /*
        FIFO implementation for controlling memory

        return: none
    */
    if (curr_frame == (map_size - 1)) {
        curr_frame = 0;
    } else {
        curr_frame++;
    }
}

int allocate_in_memory(int page, int pid) {
    /*
        Control of memory map
        Allocate a page and pid to a frame

        return: frame
    */
    memory_map[curr_frame].frame = curr_frame;
    memory_map[curr_frame].page = page;
    memory_map[curr_frame].pid = pid;

    manage_memory_space();
    return curr_frame;
}

void print_memory_map() {
    for(int i = 0; i < map_size; i++) {
        printf("frame: %d; page: %d; pid: %d\n", memory_map[i].frame, memory_map[i].page, memory_map[i].pid);
    }
    printf("\n");
}

int main(void) {
    FILE *file;
    int pid;
    int page;

    if ((file = fopen("/home/mmu-simulator/tests/2.in", "r")) == NULL) {
       printf("Error opening file!");
       return 1;
    }
    fscanf(file, "%d", &pid);
    printf("Processess using memory: %d\n", pid);

    init_memory();

    int frame;
    while (!feof (file)) {  // while there is content in the file...
        fscanf(file, "%d", &pid);
        fscanf(file, "%d", &page);

        frame = find_page_in_memory(page);  // check if page is already in memory
        if(frame != -1) {
            printf("Page %d in memory (frame %d)!\n", page, frame);
        } else {
            if(memory_is_full()) {
                printf("Memory is full! Frame %d was replaced (page %d - pid %d)\n", curr_frame, memory_map[curr_frame].page, memory_map[curr_frame].pid);
            }
            frame = allocate_in_memory(page, pid);

            manage_queue();
        }
        printf("Process %d accessed page %d - frame %d\n", pid, page, frame);

        sleep(1);
    }
    
    fclose(file);
    return 0;
}