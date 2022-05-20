#include "common.h"
int* oven_arr_ptr;
int *table_arr_ptr;
int oven_slot;
int type;
char current_time[100];
void bake_pizza(){
    oven_slot = find_empty_oven_slot(oven_arr_ptr);
    if(oven_slot == -1){
        printf("Oven is full\n");
        return;
    }
    oven_arr_ptr[oven_slot] = type;
    current_timestamp((char *) current_time);
    printf("%d %s Dodałem pizzę %d. Liczba pizz w piecu: %d\n",getpid(), current_time, type,  pizzas_in_oven(oven_arr_ptr));
}


void place_on_table(){
    int table_slot = find_empty_table_slot(table_arr_ptr);
    if(table_slot == -1){
        printf("Table is full\n");
        return;
    }
    table_arr_ptr[table_slot] = type;
    current_timestamp(current_time);
    printf("%d %s Wyjmuję pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d\n",getpid(), current_time, type, pizzas_in_oven(oven_arr_ptr), pizzas_on_table(table_arr_ptr));
}

int main(int argc, char**argv){


    sem_t* pizzas_in_oven_sem = sem_open(PIZZAS_IN_OVEN_SEM, O_RDWR);
    sem_t* pizzas_on_table_sem = sem_open(PIZZAS_ON_TABLE_SEM, O_RDWR);
    sem_t* oven_use_sem = sem_open(OVEN_USE_SEM, O_RDWR);
    sem_t* table_use_sem = sem_open(TABLE_USE_SEM, O_RDWR);
    sem_t* empty_table_sem = sem_open(EMPTY_TABLE_SEM, O_RDWR);


    int shared_memory_oven_id = shm_open(OVEN_SHARED_MEM, O_RDWR, 0666);
    int shared_memory_table_id = shm_open(TABLE_SHARED_MEM, O_RDWR, 0666);
    oven_arr_ptr = mmap(NULL, sizeof (oven_arr), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_oven_id, 0);
    table_arr_ptr = mmap(NULL, sizeof (table_arr), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_table_id, 0);
    srand(getpid());


    while (1){
        type = (rand() %10) + 1;

        current_timestamp(current_time);
        printf("%d %s przygotowuje pizze %d \n",getpid(), current_time,  type);

        sleep(PREPARE_DURATION);
        decrement_sem(pizzas_in_oven_sem);
        decrement_sem(oven_use_sem);
        bake_pizza();
        increment_sem(oven_use_sem);

        sleep(BAKE_DURATION);
        decrement_sem(oven_use_sem);
//       Take the pizza out
        oven_arr_ptr[oven_slot] = 0;

        increment_sem(oven_use_sem);

        increment_sem(pizzas_in_oven_sem);

        decrement_sem(pizzas_on_table_sem);
        decrement_sem(table_use_sem);

        place_on_table();
        increment_sem(table_use_sem);
        increment_sem(empty_table_sem);
    }
}