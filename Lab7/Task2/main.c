#include <signal.h>
#include "common.h"

int shared_memory_oven_fd;
int shared_memory_table_fd;
void exit_handler(){
    printf("EXITING!\n");
    sem_unlink(PIZZAS_ON_TABLE_SEM);
    sem_unlink(EMPTY_TABLE_SEM);
    sem_unlink(TABLE_USE_SEM);
    sem_unlink(PIZZAS_IN_OVEN_SEM);
    shm_unlink(OVEN_SHARED_MEM);
    shm_unlink(TABLE_SHARED_MEM);
    sem_unlink(OVEN_USE_SEM);
    exit(0);

}

int main(int argc, char**argv){
    signal(SIGINT, exit_handler);
    int cooks_count = atoi(argv[1]);
    int suppliers_count = atoi(argv[2]);
    shared_memory_oven_fd = shm_open(OVEN_SHARED_MEM, O_CREAT | O_RDWR, 0666);
    shared_memory_table_fd = shm_open(TABLE_SHARED_MEM, O_CREAT | O_RDWR, 0666);
    //Set size
    ftruncate(shared_memory_oven_fd, sizeof(oven_arr));
    ftruncate(shared_memory_table_fd, sizeof(table_arr));
    int* oven_arr_ptr = mmap(NULL, sizeof(oven_arr), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_oven_fd, 0);
    int* table_arr_ptr = mmap(NULL, sizeof(table_arr), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_table_fd, 0);
    for (int i = 0; i < OVEN_SIZE; ++i) {
        oven_arr_ptr[i] = 0;
    }

    for (int i = 0; i < TABLE_SIZE; ++i) {
        table_arr_ptr[i] = 0;
    }
    printf("main: Oven: %d, Table: %d\n", shared_memory_oven_fd, shared_memory_table_fd);

//    Create the semaphores
    sem_open(PIZZAS_IN_OVEN_SEM, O_CREAT, 0666, OVEN_SIZE);
    sem_open(PIZZAS_ON_TABLE_SEM, O_CREAT, 0666, TABLE_SIZE);
    sem_open(OVEN_USE_SEM, O_CREAT, 0666, 1);
    sem_open(TABLE_USE_SEM, O_CREAT, 0666, 1);
    sem_open(EMPTY_TABLE_SEM, O_CREAT, 0666, 0);

//    Setup the cooks and suppliers
    for (int i = 0; i < cooks_count; ++i) {
        if(fork() == 0){
            execl("./cook", "cook", NULL);
        }
    }
    for (int i = 0; i < suppliers_count; ++i) {
        if(fork() == 0){
            execl("./supplier", "supplier", NULL);
        }
    }
    while(1){
        sleep(1);
    }
}

