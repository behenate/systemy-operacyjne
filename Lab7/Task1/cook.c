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
    int sem_id = get_sem_id();
    int shared_memory_oven_id = get_shared_memory_oven_id();
    int shared_memory_table_id = get_shared_memory_table_id();
    oven_arr_ptr = shmat(shared_memory_oven_id, NULL, 0);
    table_arr_ptr = shmat(shared_memory_table_id, NULL, 0);
    srand(getpid());
    while (1){
        type = (rand() %10) + 1;

        current_timestamp(current_time);
        printf("%d %s przygotowuje pizze %d \n",getpid(), current_time,  type);

        sleep(PREPARE_DURATION);
        decrement_sem(sem_id, PIZZAS_IN_OVEN_SEM);
        decrement_sem(sem_id, OVEN_USE_SEM);
        bake_pizza();
        increment_sem(sem_id, OVEN_USE_SEM);

        sleep(BAKE_DURATION);
        decrement_sem(sem_id, OVEN_USE_SEM);
        oven_arr_ptr[oven_slot] = 0;
        increment_sem(sem_id, OVEN_USE_SEM);

        increment_sem(sem_id, PIZZAS_IN_OVEN_SEM);

        decrement_sem(sem_id, PIZZAS_ON_TABLE_SEM);
        decrement_sem(sem_id, TABLE_USE_SEM);

        place_on_table();
        increment_sem(sem_id, TABLE_USE_SEM);
        increment_sem(sem_id, EMPTY_TABLE_SEM);
    }
}