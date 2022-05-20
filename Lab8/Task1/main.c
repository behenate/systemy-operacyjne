#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

FILE * input;
FILE * output;
int num_threads;
int w;
int h;
int max_brightness;
int ** matrix;
int ** negative;
int processed_cnt =0;
void save_negative(){
    fprintf(output, "%s\n", "P2");
    fprintf(output, "%d %d\n", w, h);
    fprintf(output, "%d\n", max_brightness);
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            fprintf(output, "%d\n", negative[i][j]);
        }
    }
}

void* process_input_numbers_thread(void* arg){
    struct timeval st1, et1;
    gettimeofday(&st1,NULL);
    int thread_id = *(int*)arg;
    int start_brightness = max_brightness / num_threads * thread_id;
    int end_brightness = max_brightness / num_threads * (thread_id + 1);
    if (thread_id == num_threads - 1){
        end_brightness = max_brightness+1;
    }

    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            if (matrix[i][j] >= start_brightness && matrix[i][j] < end_brightness){
                negative[i][j] = max_brightness-matrix[i][j];
            }
        }
    }
    gettimeofday(&et1,NULL);
    size_t elapsed = ((et1.tv_sec - st1.tv_sec) * 1000000) + (et1.tv_usec - st1.tv_usec);
    printf("Processing time, thread %d : %d micro seconds\n",thread_id, elapsed);
    pthread_exit(NULL);
}

void * process_input_blocks_thread(void* arg){
    struct timeval st, et;

    gettimeofday(&st,NULL);
    int thread_id = *(int*)arg;
    int start_column = w / num_threads * thread_id;
    int end_column = w / num_threads * (thread_id + 1);
    if (thread_id == num_threads - 1){
        end_column = w+1;
    }
    for (int i = 0; i < h; ++i) {
        for (int j = start_column; j < end_column; ++j) {
            negative[i][j] = max_brightness - matrix[i][j];
        }
    }
    gettimeofday(&et,NULL);
    int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
    printf("Processing time, thread %d : %d micro seconds\n",thread_id, elapsed);

    pthread_exit(NULL);
}

void process_input_numbers(){
    struct timeval st, et;

    gettimeofday(&st,NULL);
    pthread_t threads[num_threads];
    int threads_id[num_threads];
    for (int i = 0; i < num_threads; ++i) {
        threads_id[i] = i;
        pthread_create(&threads[i], NULL, &process_input_numbers_thread, &threads_id[i]);
    }
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], (void**)NULL);
    }
    gettimeofday(&et,NULL);
    int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
    printf("Processing time, numbers mode: %d micro seconds\n",elapsed);
}
void process_input_blocks(){
    struct timeval st, et;

    gettimeofday(&st,NULL);
    pthread_t threads[num_threads];
    int threads_id[num_threads];
    for (int i = 0; i < num_threads; ++i) {
        threads_id[i] = i;
        pthread_create(&threads[i], NULL, &process_input_blocks_thread, &threads_id[i]);
    }
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], (void**)NULL);
    }
    gettimeofday(&et,NULL);
    int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
    printf("Processing time, blocks mode: %d micro seconds\n",elapsed);
}
int main(int argc, char** argv){
    if(argc != 5){
        perror("Wrong number of arguments");
    }
    num_threads = atoi(argv[1]);
    char* mode_name = argv[2];
    char* file_name = argv[3];
    char* output_name = argv[4];
    input = fopen(file_name, "r");
    output = fopen(output_name, "w+");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    getline(&line, &len, input);
    getline(&line, &len, input);
    sscanf(line, "%d %d", &w, &h);
    getline(&line, &len, input);
    sscanf(line, "%d", &max_brightness);
    matrix = (int**)malloc(h * sizeof(int*));
    negative = (int**)malloc(h * sizeof(int*));
    for (int i = 0; i < h; ++i) {
        matrix[i] = (int*)malloc(w * sizeof(int));
        negative[i] = (int*)malloc(w * sizeof(int));
    }
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            getline(&line, &len, input);
            matrix[i][j] = atoi(line);
        }
    }
    printf("%d %d\n", w, h);
    if (strcmp(mode_name, "numbers") == 0){
        process_input_numbers();
    }else if (strcmp(mode_name, "block") == 0){
        printf("%d\n", num_threads);
        process_input_blocks();
    }else{
        perror("Wrong mode name, please use either numbers or block");
    }
    save_negative();
    fclose(input);
    fclose(output);

}