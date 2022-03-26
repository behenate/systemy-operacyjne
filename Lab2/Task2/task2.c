#include "task2.h"

// Funkcja zliczająca przy pomocy biblioteki
void count(FILE *file, char c){
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    int lines_cnt = 0;
    int total_cnt = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        int res = count_char(line, read, c);
        if(res > 0){
            lines_cnt ++;
            total_cnt+=res;
        }
    }
    printf("Lines count: %d \n", lines_cnt);
    printf("Total count: %d \n", total_cnt);
}


// Funkcja zliczającą wystąpienia w podanym ciągu znaków
int count_char(char *line, int length, char c){
    int count = 0;
    for (int i = 0; i < length; ++i) {
        if(line[i] == c){
            count++;
        }
    }
    return count;
}

// FUnkcja zliczająca przy użyciu funkcji systemowych
void count_sys(int file, char counting){
    char c;
    char *line = calloc(5000, sizeof (char));
    int line_len = 0;

    int lines_cnt = 0;
    int total_cnt = 0;

    while(read(file,&c,1)==1){
        line[line_len] = c;
        if (c == '\n'){
            int cnt = count_char(line, line_len, counting);
            if (cnt > 0){
                lines_cnt ++;
                total_cnt += cnt;
            }
            line = calloc(256, sizeof (char));
            line_len = 0;
        }else{
            line_len++;
        }
    }
    printf("Lines count: %d \n", lines_cnt);
    printf("Total count: %d \n", total_cnt);
}
