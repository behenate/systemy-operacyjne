#include "task3.h"
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int isText(FILE *pfFile) {
    fseek(pfFile, 0, SEEK_SET);
    int byte_cnt = 0;
    int ascii_cnt = 0;
    int cVal = 0;
    while((cVal = fgetc(pfFile))!=EOF && byte_cnt < 10000) {
        byte_cnt++;
        ascii_cnt += (cVal > 31 && cVal < 127);
    }
    int proportion = ((ascii_cnt * 100) / byte_cnt);
    return (proportion >= 50);
}


int look_for_str(char *filename, char *substring) {
    FILE *file = fopen(filename, "r");
    char * buffer = 0;

    fseek (file, 0, SEEK_END);
    long length = ftell (file);
    fseek (file, 0, SEEK_SET);
    buffer = malloc (length);
    fread (buffer, 1, length, file);

    if (strstr(buffer, substring)){
        return 1;
    }


    fclose(file);
    return 0;
}

void browse_directory(DIR *dir, char *path, int depth_left, char* substring) {
    if (depth_left <= 0) {
        return;
    }
    struct dirent *single_res = readdir(dir);
    printf("\t Path: %s \n", path);
    printf("\t Child pid %d parent pid %d\n", getpid(), getppid());
    readdir(dir);

    while (single_res != NULL) {
        int is_dir = 0;
        struct stat file_info;
        if (strcmp(single_res->d_name, ".") == 0 || strcmp(single_res->d_name, "..") == 0) {
            single_res = readdir(dir);
            continue;
        }

        char *res_path = calloc(strlen(path) + 50, sizeof(char));
        sprintf(res_path, "%s/%s", path, single_res->d_name);

        lstat(res_path, &file_info);



        if (S_ISREG(file_info.st_mode)) {
            FILE * file = fopen(res_path, "r");
            int is_text_file = isText(file);
            if (is_text_file){
                if (look_for_str(res_path, substring)){
                    printf("\t\t file: %s: found the phrase! :))\n", single_res->d_name);
                }else{
                    printf("\t\t file: %s The phrase has not been found :(( \n", single_res->d_name);
                }
            }
            fclose(file);

        } else if (S_ISDIR(file_info.st_mode)) {
            is_dir = 1;
        }


        if (is_dir) {
            DIR *new_dir = opendir(res_path);
            pid_t child_pid = fork();
            if(child_pid == 0){
                printf("\n Searching in directory: %s\n", single_res->d_name);
                browse_directory(new_dir, res_path, depth_left - 1, substring);
                exit(0);
            }
            wait(NULL);
            closedir(new_dir);
        }
        single_res = readdir(dir);
    }

}