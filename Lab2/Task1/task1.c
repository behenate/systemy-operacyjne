#include "task1.h"

int is_whitespaces(char *line, int length){
    char whitespace_chars[] = {' ' , '\t', '\n'};
    for (int i = 0; i < length; ++i) {
        int is_ok = 0;
        for (int j = 0; j < 4; ++j) {
            if(line[i] == whitespace_chars[j]){
                is_ok = 1;
            }
        }
        if(!is_ok){
            return 0;
        }
    }
    return 1;
}


void copy_remove_whitespace(FILE *file, FILE *target_file){
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        if(!is_whitespaces(line, read)){
            fprintf(target_file, "%s", line);
        }
    }
}

void copy_remove_whitespace_sys(int file,int target_file){
    char c;
    char *line = calloc(256, sizeof (char));
    int line_len = 0;
    while(read(file,&c,1)==1){
        line[line_len] = c;
        if (c == '\n'){
            int is_whitespace = is_whitespaces(line, line_len);
            if (!is_whitespace){
                write(target_file,line,(line_len+1)*sizeof (char ));
            }
            line = calloc(256, sizeof (char));
            line_len = 0;
        }else{
            line_len++;
        }

    }
    free(line);


//    FILE * fp;
//    char * line = NULL;
//    size_t len = 0;
//    ssize_t read;
//
//    while ((read = getline(&line, &len, file)) != -1) {
//        if(!is_whitespaces(line, read)){
//            fprintf(target_file, "%s", line);
//        }
//    }
}