#ifndef SYSOPY_COMMON_H
#define SYSOPY_COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/mman.h>


#define OVEN_SIZE 5
#define TABLE_SIZE 5

#define OVEN_USE_SEM "oven_use_sem"
#define TABLE_USE_SEM "table_use_sem"
#define PIZZAS_IN_OVEN_SEM "pizzas_in_oven_sem"
#define PIZZAS_ON_TABLE_SEM "pizzas_on_table_sem"
#define EMPTY_TABLE_SEM "empty_table_sem"


#define DELIVER_LENGTH 5
#define RETURN_LENGTH 4
#define PREPARE_DURATION 1
#define BAKE_DURATION 5

#define OVEN_SHARED_MEM "oven_shared_memory"
#define TABLE_SHARED_MEM "table_shared_memory"

int oven_arr[OVEN_SIZE];
int table_arr[TABLE_SIZE];

void increment_sem(sem_t* sem){
    if(sem_post(sem) == -1){
        perror("sem_post");
        exit(EXIT_FAILURE);
    };
}

void decrement_sem(sem_t* sem){
    if(sem_wait(sem) == -1){
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
}


int get_shared_memory_oven_fd(){
    int fd = shm_open(OVEN_SHARED_MEM, O_RDWR, 0666);
    return fd;
}
int get_shared_memory_table_fd(){
    int fd = shm_open(TABLE_SHARED_MEM, O_RDWR, 0666);
    return fd;
}
int find_empty_oven_slot(const int* oven_arr_ptr){
    for(int i = 0; i < OVEN_SIZE; i++){
        if(oven_arr_ptr[i] == 0){
            return i;
        }
    }
    return -1;
}
int find_empty_table_slot(const int *table_arr_ptr){
    for(int i = 0; i < TABLE_SIZE; i++){
        if(table_arr_ptr[i] == 0){
            return i;
        }
    }
    return -1;
}

int find_full_table_slot(const int *table_arr_ptr){
    for(int i = 0; i < TABLE_SIZE; i++){
        if(table_arr_ptr[i] != 0){
            return i;
        }
    }
    return -1;
}


void current_timestamp(char *buffer){
    struct timeval tp;
    gettimeofday(&tp, 0);
    time_t curtime = tp.tv_sec;
    struct tm *t = localtime(&curtime);
//    printf("%02d:%02d:%02d:%03d\n", t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
    sprintf(buffer, "%02d:%02d:%02d:%03ld", t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
}

int pizzas_in_oven(const int* oven_arr_ptr){
    int pizzas_in_oven = 0;
    for(int i = 0; i < OVEN_SIZE; i++){
        if(oven_arr_ptr[i] != 0){
            pizzas_in_oven++;
        }
    }
    return pizzas_in_oven;
}
int pizzas_on_table(const int* table_arr_ptr){
    int pizzas_on_table = 0;
    for(int i = 0; i < TABLE_SIZE; i++){
        if(table_arr_ptr[i] != 0){
            pizzas_on_table++;
        }
    }
    return pizzas_on_table;
}

#endif //SYSOPY_COMMON_H
