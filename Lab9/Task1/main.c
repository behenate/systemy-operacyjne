#define ELVES_NUM 10
#define REINDEER_NUM 9
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
int elves_waiting = 0;
int reindeer_waiting = 0;

int elves_queue[ELVES_NUM];

int presents= 0;
//halo

pthread_mutex_t elves_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t elves_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t elves_wait_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t reindeer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reindeer_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t reindeer_wait_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;

void* elves_work(void* arg){
    int id = *(int*)arg;
    while (1){

        sleep(rand() % 4 + 2);
        pthread_mutex_lock(&elves_wait_mutex);
        while (elves_waiting == 3){
            pthread_cond_wait(&elves_cond, &elves_wait_mutex);
        }
        pthread_mutex_unlock(&elves_wait_mutex);

        pthread_mutex_lock(&elves_mutex);

        if(elves_waiting < 3){
            elves_queue[elves_waiting] = id;
            elves_waiting++;
        }
        printf("Elf: czeka %d elfoów na mikołaja %d \n", elves_waiting, id);


        if(elves_waiting == 3){
            printf("Elf: wybudzam Mikołaja, ID: %d \n", id);
            pthread_mutex_lock(&santa_mutex);
            pthread_cond_broadcast(&santa_cond);
            pthread_mutex_unlock(&santa_mutex);
        }

        pthread_mutex_unlock(&elves_mutex);
        sleep(rand()%2 + 1);

    }
    return 0;
}

void* reindeer_work(void* arg){
    int id = *(int*)arg;
    while (1){
        sleep(rand()% 6 + 5);
        pthread_mutex_lock(&reindeer_wait_mutex);
        while (reindeer_waiting == 9){
            pthread_cond_wait(&reindeer_cond, &reindeer_wait_mutex);
        }
        pthread_mutex_unlock(&reindeer_wait_mutex);

        pthread_mutex_lock(&reindeer_mutex);
        reindeer_waiting ++;
        printf("Reifer: czeka %d reniferów na mikołaja, %d \n", reindeer_waiting, id);
        if(reindeer_waiting == 9){
            printf("Reifer: wybudzam Mikołaja, ID: %d\n", id);
            pthread_mutex_lock(&santa_mutex);
            pthread_cond_broadcast(&santa_cond);
            pthread_mutex_unlock(&santa_mutex);
        }
        printf("Reifer: dostarczam, ID: %d\n", id);
        pthread_mutex_unlock(&reindeer_mutex);
        sleep(rand()%3 + 1);
    }
    return 0;
}

void* santa_work(){
    while (1){
        pthread_mutex_lock(&santa_mutex);
        while (elves_waiting < 3 && reindeer_waiting < 9){
            pthread_cond_wait(&santa_cond, &santa_mutex);
        }
        pthread_mutex_unlock(&santa_mutex);

        printf("Mikołaj: budzę się\n");
        pthread_mutex_lock(&reindeer_mutex);
        if(reindeer_waiting == 9){
            printf("Mikołaj: dostarczam zabawki \n");
            sleep(rand()%3 + 2);
            reindeer_waiting = 0;
            pthread_mutex_lock(&reindeer_wait_mutex);
            pthread_cond_broadcast(&reindeer_cond);
            pthread_mutex_unlock(&reindeer_wait_mutex);
            presents++;
        }
        if(presents==3){
            printf("Fin \n");
            break;
        }

        pthread_mutex_unlock(&reindeer_mutex);

        pthread_mutex_lock(&elves_mutex);
        if (elves_waiting == 3){
            printf("Mikołaj: rozwiązuje problemy elfów ");
            for (int i = 0; i < ELVES_NUM; ++i) {
                if(elves_queue[i] != -1){
                    printf("%d ", elves_queue[i]);
                }
            }
            printf("\n");
            sleep(rand() %2 + 1);

            pthread_mutex_lock(&elves_wait_mutex);
            for (int i = 0; i < ELVES_NUM; ++i) {
                elves_queue[i] = -1;
            }
            elves_waiting -= 3;
            pthread_cond_broadcast(&elves_cond);
            pthread_mutex_unlock(&elves_wait_mutex);
        }

        printf("Mikołaj: zasypiam\n");
        pthread_mutex_unlock(&elves_mutex);
    }
    return 0;
}
int main(int argc, char** argv){
    for (int i = 0; i < ELVES_NUM; ++i) {
        elves_queue[i] = -1;
    }
    pthread_t santa_thread;
    pthread_create(&santa_thread, NULL, santa_work, NULL);
    int* reindeers_id = malloc(sizeof(int) * REINDEER_NUM);
    int* elves_id = malloc(sizeof(int) * ELVES_NUM);

    pthread_t* reindeer_threads = calloc(REINDEER_NUM, sizeof(pthread_t));
    pthread_t* elves_threads = calloc(ELVES_NUM, sizeof(pthread_t));
    for (int i = 0; i < REINDEER_NUM; ++i) {
        reindeers_id[i] = i;
        pthread_create(&reindeer_threads[i], NULL, reindeer_work, &reindeers_id[i]);
    }
    for (int i = 0; i < ELVES_NUM; ++i) {
        elves_id[i] = i;
        pthread_create(&elves_threads[i], NULL, elves_work, &elves_id[i]);
    }
    pthread_join(santa_thread, NULL);
    for (int i = 0; i < REINDEER_NUM; ++i) {
        pthread_join(reindeer_threads[i], NULL);
    }
    for (int i = 0; i < ELVES_NUM; ++i) {
        pthread_join(elves_threads[i], NULL);
    }
    free(reindeers_id);
    free(elves_id);
    free(reindeer_threads);
    free(elves_threads);
    return 0;
}