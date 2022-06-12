#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int elves_waiting = 0;
int reindeer_waiting = 0;
int elves_queue[10];
int presents= 0;

pthread_mutex_t santa_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t elves_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reindeer_m = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t elves_waiting_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reindeer_waiting_m = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t santa_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t elves_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t reindeer_c = PTHREAD_COND_INITIALIZER;




void wake_up_santa(){
    pthread_mutex_lock(&santa_m);
    pthread_cond_broadcast(&santa_c);
    pthread_mutex_unlock(&santa_m);
}

void elf_waiting_routine(int elf_no){
    if(elves_waiting < 3){
        elves_queue[elves_waiting] = elf_no;
        elves_waiting++;
        printf("Elf: czeka %d elfoów na mikołaja %d \n", elves_waiting, elf_no);

    }else if(elves_waiting == 3){
        printf("Elf: wybudzam Mikołaja, ID: %d \n", elf_no);
        wake_up_santa();
        printf("Elf: Mikołaj rozwiązuje problem, ID: %d \n", elf_no);
    }else{
        printf("Elf: czeka na powrót elfów, ID: %d \n", elf_no);
    }
}
void* elves_work(void* arg){
    int elf_no = *(int*)arg;
    while (1){
        sleep(rand() % 4 + 2);
        pthread_mutex_lock(&elves_waiting_m);
        while (elves_waiting == 3)
            pthread_cond_wait(&elves_c, &elves_waiting_m);

        pthread_mutex_unlock(&elves_waiting_m);

        pthread_mutex_lock(&elves_m);
        elf_waiting_routine(elf_no);
        pthread_mutex_unlock(&elves_m);
        sleep(rand()%2 + 1);
    }
}
void reindeer_waiting_routine(int reindeer_no){
    reindeer_waiting ++;
    printf("Reifer: czeka %d reniferów na mikołaja, %d \n", reindeer_waiting, reindeer_no);
    if(reindeer_waiting == 9){
        printf("Reifer: wybudzam Mikołaja, ID: %d\n", reindeer_no);
        wake_up_santa();
    }
    printf("Reifer: dostarczam, ID: %d\n", reindeer_no);
}
void* reindeer_work(void* arg){
    int reindeer_no = *(int*)arg;
    while (1){
        sleep(rand()% 6 + 5);
        pthread_mutex_lock(&reindeer_waiting_m);
        while (reindeer_waiting == 9)
            pthread_cond_wait(&reindeer_c, &reindeer_waiting_m);

        pthread_mutex_unlock(&reindeer_waiting_m);

        pthread_mutex_lock(&reindeer_m);
        reindeer_waiting_routine(reindeer_no);
        pthread_mutex_unlock(&reindeer_m);
        sleep(rand()%3 + 1);
    }
}

void* santa_work(){
    while (1){
        pthread_mutex_lock(&santa_m);
        while (elves_waiting < 3 && reindeer_waiting < 9)
            pthread_cond_wait(&santa_c, &santa_m);
        pthread_mutex_unlock(&santa_m);

        printf("Mikołaj: budzę się\n");
        pthread_mutex_lock(&reindeer_m);
        if(reindeer_waiting == 9){
            printf("Mikołaj: dostarczam zabawki \n");
            sleep(rand()%3 + 2);
            reindeer_waiting = 0;
            pthread_mutex_lock(&reindeer_waiting_m);
            pthread_cond_broadcast(&reindeer_c);
            pthread_mutex_unlock(&reindeer_waiting_m);
            presents++;
            if(presents==3){
                printf("Christmas is over :(( \n");
                break;
            }
        }

        pthread_mutex_unlock(&reindeer_m);

        pthread_mutex_lock(&elves_m);
        if (elves_waiting == 3){
            printf("Mikołaj: rozwiązuje problemy elfów ");
            for (int i = 0; i < 10; ++i) {
                if(elves_queue[i] != -1){
                    printf("%d ", elves_queue[i]);
                }
            }
            printf("\n");
            sleep(rand() %2 + 1);

            pthread_mutex_lock(&elves_waiting_m);
            for (int i = 0; i < 10; ++i) {
                elves_queue[i] = -1;
            }
            elves_waiting -= 3;
            pthread_cond_broadcast(&elves_c);
            pthread_mutex_unlock(&elves_waiting_m);
        }

        printf("Mikołaj: zasypiam\n");
        pthread_mutex_unlock(&elves_m);
    }
    return 0;
}
int main(int argc, char** argv){
    for (int i = 0; i < 10; ++i) {
        elves_queue[i] = -1;
    }
    pthread_t santa_thread;
    pthread_create(&santa_thread, NULL, santa_work, NULL);
    int* reindeers_id = malloc(sizeof(int) * 9);
    int* elves_id = malloc(sizeof(int) * 10);

    pthread_t* reindeer_threads = calloc(9, sizeof(pthread_t));
    pthread_t* elves_threads = calloc(10, sizeof(pthread_t));
    for (int i = 0; i < 9; ++i) {
        reindeers_id[i] = i;
        pthread_create(&reindeer_threads[i], NULL, reindeer_work, &reindeers_id[i]);
    }
    for (int i = 0; i < 10; ++i) {
        elves_id[i] = i;
        pthread_create(&elves_threads[i], NULL, elves_work, &elves_id[i]);
    }
    pthread_join(santa_thread, NULL);
    for (int i = 0; i < 9; ++i) {
        pthread_join(reindeer_threads[i], NULL);
    }
    for (int i = 0; i < 10; ++i) {
        pthread_join(elves_threads[i], NULL);
    }
    free(reindeers_id);
    free(elves_id);
    free(reindeer_threads);
    free(elves_threads);
    return 0;
}
