#ifndef SYSOPY_COMMON_H
#define SYSOPY_COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>



#define PROJECT_ID 123
#define OVEN_ID 1234
#define TABLE_ID 12345

#define OVEN_PATH "cook"
#define TABLE_PATH "supplier"

#define OVEN_SIZE 5
#define TABLE_SIZE 5

#define OVEN_USE_SEM 0
#define TABLE_USE_SEM 1
#define PIZZAS_IN_OVEN_SEM 2
#define PIZZAS_ON_TABLE_SEM 3
#define EMPTY_TABLE_SEM 4


#define DELIVER_LENGTH 5
#define RETURN_LENGTH 4
#define PREPARE_DURATION 1
#define BAKE_DURATION 5


int oven_arr[OVEN_SIZE];
int table_arr[TABLE_SIZE];
char* get_home_path(){
    char* path = getenv("HOME");
    return path;
}

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
}arg;

void increment_sem(int sem_id, int sem_num){
    struct sembuf s = {.sem_num = sem_num, .sem_op = 1, .sem_flg = 0};
    semop(sem_id, &s, 1);
}

void decrement_sem(int sem_id, int sem_num){
    struct sembuf s = {.sem_num = sem_num, .sem_op = -1, .sem_flg = 0};
    semop(sem_id, &s, 1);
}

int get_sem_id(){
    key_t key = ftok(get_home_path(), PROJECT_ID);
    int sem_id = semget(key, 5, 0);
    return sem_id;
}
int get_shared_memory_oven_id(){
    key_t key = ftok(OVEN_PATH, OVEN_ID);
    int shm_id = shmget(key, sizeof(oven_arr), 0);
    return shm_id;
}
int get_shared_memory_table_id(){
    key_t key = ftok(TABLE_PATH, TABLE_ID);
    int shm_id = shmget(key, sizeof(table_arr), 0);
    return shm_id;
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
