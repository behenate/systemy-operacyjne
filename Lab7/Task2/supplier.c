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
    sem_t* pizzas_on_table_sem = sem_open(PIZZAS_ON_TABLE_SEM, O_RDWR);
    sem_t* table_use_sem = sem_open(TABLE_USE_SEM, O_RDWR);
    sem_t* empty_table_sem = sem_open(EMPTY_TABLE_SEM, O_RDWR);

    int shared_memory_table_id = get_shared_memory_table_fd();
    table_arr_ptr = mmap(NULL, sizeof (table_arr), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_table_id, 0);
    while (1) {
        decrement_sem(empty_table_sem);
        decrement_sem(table_use_sem);
        take_pizza_from_table();
        increment_sem(table_use_sem);
        increment_sem(pizzas_on_table_sem);

        sleep(DELIVER_LENGTH);
        current_timestamp(current_time);
        printf("%d %s Dostarczam pizzę %d.\n", getpid(), current_time, type);
        sleep(RETURN_LENGTH);
    }
}