#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>

int add_text_to_line(char* filename, char* text,  int line_no){
    FILE * file;
    FILE * fTemp;

    char buffer[10000];
    int count;

    file  = fopen(filename, "r");
    fTemp = fopen("replace.tmp", "w");
    flock(fileno(file), LOCK_EX);
    flock(fileno(fTemp), LOCK_EX);
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int found = 0;
    char * replace_with = calloc(10000, sizeof (char ));
    int lines_cnt = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        if(lines_cnt == line_no){
            found = 1;
            sprintf(replace_with, "%s%s",line, text);
        }
        lines_cnt++;
    }
    if (!found){
        sprintf(replace_with, "%s", text);
    }


    fclose(file);
    file = fopen(filename, "r");
    flock(fileno(file), LOCK_EX);
    if(lines_cnt > line_no+1){
        count = 0;
        while ((fgets(buffer, 10000, file)) != NULL)
        {
            if (count == line_no)
                fputs(replace_with, fTemp);
            else
                fputs(buffer, fTemp);
            count++;
        }
    }else{
        for (int i = 0; i < line_no; ++i) {
            if((fgets(buffer, 10000, file)) != NULL){
                fputs(buffer, fTemp);
            }else{
                fputs("\n", fTemp);
            }
        }
        fputs(replace_with, fTemp);
    }


    flock(fileno(file), LOCK_UN);
    flock(fileno(fTemp), LOCK_UN);
    fclose(file);
    fclose(fTemp);
    remove(filename);
    rename("replace.tmp", filename);


    return 0;
}

int main(int argc, char **argv) {
    printf("Start consumer! \n");
    char *stream_path = calloc(1000, sizeof(char));
    int line_no = 0;
    char *file_path = calloc(1000, sizeof(char));
    int n = 0;


    if (argc == 4) {
        stream_path = argv[1];
        file_path = argv[2];
        n = atoi(argv[3]);

    }
//    Create the file if not already created


    char * buffer = calloc(n+5, sizeof (char ));
    FILE* stream = fopen(stream_path, "r");

    while(fread(buffer, sizeof(char), n+5, stream) == n+5){
        char * line_scr = calloc(5, sizeof (char));
        char * line = calloc(n, sizeof (char ));

        for (int i = 0; i < 5; ++i) {
            line_scr[i] = buffer[i];
        }
        for (int i = 0; i < n; ++i) {
            line[i] = buffer[i+5];
        }

        add_text_to_line(file_path, line, atoi(line_scr) - 90000 - 1);
//
//        char* seq = strtok(buffer, ".");   // separate number of producer (before the dot)
//        int num = atoi(seq);
//
//        seq = strtok(NULL, "\n");
//
//        char* line;
//        sprintf(line, "%s", seq);
//
//
//        save_to_file(file, line, num);
    }
    fclose(stream);
    return 0;
}