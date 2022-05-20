#include "common.h"
#include <signal.h>
int shared_memory_oven_id;
int shared_memory_table_id;
int sem_id;
void exit_handler(){
    printf("Exiting \n");
    semctl(sem_id, 0, IPC_RMID, NULL);
    shmctl(shared_memory_oven_id, IPC_RMID, NULL);
    shmctl(shared_memory_table_id, IPC_RMID, NULL);
    exit(1);
}

int main(int argc, char**argv){
    int cooks_count = atoi(argv[1]);
    int suppliers_count = atoi(argv[2]);
    signal(SIGINT, exit_handler);
//    Create shared memory for oven and table
    key_t oven_key = ftok(OVEN_PATH, OVEN_ID);
    key_t table_key = ftok(TABLE_PATH, TABLE_ID);
    shared_memory_oven_id = shmget(oven_key, sizeof(oven_arr), IPC_CREAT | 0666);
    shared_memory_table_id = shmget(table_key, sizeof(table_arr), IPC_CREAT | 0666);
    int* oven_arr_ptr = shmat(shared_memory_oven_id, NULL, 0);
    int* table_arr_ptr = shmat(shared_memory_table_id, NULL, 0);
    for (int i = 0; i < OVEN_SIZE; ++i) {
        oven_arr_ptr[i] = 0;
    }

    for (int i = 0; i < TABLE_SIZE; ++i) {
        table_arr_ptr[i] = 0;
    }
//    Create the semaphores
    key_t sem_key = ftok(get_home_path(), PROJECT_ID);
    sem_id = semget(sem_key, 5, IPC_CREAT | 0666);
    union arg;
    arg.val = 1;
    semctl(sem_id, OVEN_USE_SEM, SETVAL, arg);
    semctl(sem_id, TABLE_USE_SEM, SETVAL, arg);
    arg.val = OVEN_SIZE;
    semctl(sem_id, PIZZAS_IN_OVEN_SEM, SETVAL, arg);
    arg.val = TABLE_SIZE;
    semctl(sem_id, PIZZAS_ON_TABLE_SEM, SETVAL, arg);
    arg.val = 0;
    semctl(sem_id, EMPTY_TABLE_SEM, SETVAL, arg);

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

