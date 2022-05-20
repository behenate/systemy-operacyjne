#include "common.h"

int *table_arr_ptr;
int type;
char current_time[100];

void take_pizza_from_table() {
    int slot = find_full_table_slot(table_arr_ptr);
    type = table_arr_ptr[slot];
    table_arr_ptr[slot] = 0;
    current_timestamp(current_time);
    printf("%d %s Pobieram pizzę %d liczba pizz na stole %d.\n", getpid(), current_time, type,
           pizzas_on_table(table_arr_ptr));
}

int main(int argc, char **argv) {
    int sem_id = get_sem_id();
    int shared_memory_table_id = get_shared_memory_table_id();
    table_arr_ptr = shmat(shared_memory_table_id, NULL, 0);

    while (1) {
        decrement_sem(sem_id, EMPTY_TABLE_SEM);
        decrement_sem(sem_id, TABLE_USE_SEM);
        take_pizza_from_table();
        increment_sem(sem_id, TABLE_USE_SEM);
        increment_sem(sem_id, PIZZAS_ON_TABLE_SEM);

        sleep(DELIVER_LENGTH);
        current_timestamp(current_time);
        printf("%d %s Dostarczam pizzę %d.\n", getpid(), current_time, type);
        sleep(RETURN_LENGTH);
    }
}