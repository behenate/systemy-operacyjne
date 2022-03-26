#define _XOPEN_SOURCE 500

#include <libzvbi.h>
#include <string.h>
#include <malloc.h>
#include <sys/stat.h>
#include <ftw.h>
#include "task3.h"

int file_num = 0;
int dir_num = 0;
int char_dev_num = 0;
int block_num = 0;
int fifo_num = 0;
int symlink_num = 0;
int socket_num = 0;

void reset_cnts(){
    file_num = 0;
    dir_num = 0;
    char_dev_num = 0;
    block_num = 0;
    fifo_num = 0;
    symlink_num = 0;
    socket_num = 0;
}

void print_cnts(){
    printf("Files:        %d\n", file_num);
    printf("Directories:  %d\n", dir_num);
    printf("Char devs:    %d\n", char_dev_num);
    printf("Block devs:   %d\n", block_num);
    printf("FIFOs:        %d\n", fifo_num);
    printf("Symlinks:     %d\n", symlink_num);
    printf("Sockets:      %d\n", socket_num);

}

void browse_directory(DIR *dir, char *absolute_path) {
    struct dirent *single_res = readdir(dir);
    readdir(dir);
    while (single_res != NULL) {
        int is_dir = 0;
        struct stat file_info;
        if (strcmp(single_res->d_name, ".") == 0 || strcmp(single_res->d_name, "..") == 0) {
            single_res = readdir(dir);
            continue;
        }
        printf("\nFILE: %s:\n", single_res->d_name);
        char *res_path = calloc(strlen(absolute_path) + 50, sizeof(char));
        for (int i = 0; i < strlen(absolute_path); ++i) {
            res_path[i] = absolute_path[i];
        }
        strcat(res_path, "/");
        strcat(res_path, single_res->d_name);

        printf("\tPath : %s \n", res_path);

        lstat(res_path, &file_info);

        if (S_ISREG(file_info.st_mode)) {
            printf("\tType: file\n");
            file_num++;
        } else if (S_ISDIR(file_info.st_mode)) {
            printf("\tType: directory\n");
            is_dir = 1;
            dir_num++;
        } else if (S_ISCHR(file_info.st_mode)) {
            printf("\tType: char dev\n");
            char_dev_num++;
        } else if (S_ISBLK(file_info.st_mode)) {
            printf("\tType: block special\n");
            block_num++;
        } else if (S_ISFIFO(file_info.st_mode)) {
            printf("\tType: fifo\n");
            fifo_num++;
        } else if (S_ISLNK(file_info.st_mode)) {
            printf("\tType: symbolic link\n");
            symlink_num++;
        } else if (S_ISSOCK(file_info.st_mode)) {
            printf("\tType: socket\n");
            socket_num++;
        }

        printf("\t Number of links: %lu \n", file_info.st_nlink);
        printf("\t Size: %lu \n", file_info.st_size);
        printf("\t Last access: %lu \n", file_info.st_atime);
        printf("\t Modification time: %lu \n", file_info.st_mtime);
        if (is_dir){
            DIR *test = opendir(res_path);
            browse_directory(test, res_path);
        }
        single_res = readdir(dir);
    }
    closedir(dir);
}

int nftw_callback(const char *path, const struct stat *file_info, int flags, struct FTW *ftw) {
    int path_length = strlen(path);
    char *filename = calloc(path_length - ftw->base, sizeof(char));
    for (int i = ftw->base; i < path_length; ++i) {
        filename[i - ftw->base] = path[i];
    }

    printf("\nFILE: %s \n", filename);
    printf("Path: %s \n", path);
    printf("Path: %s \n", path);

    if (S_ISREG(file_info->st_mode)) {
        printf("\tType: file\n");
        file_num++;
    } else if (S_ISDIR(file_info->st_mode)) {
        printf("\tType: directory\n");
        dir_num++;
    } else if (S_ISCHR(file_info->st_mode)) {
        printf("\tType: char dev\n");
        char_dev_num++;
    } else if (S_ISBLK(file_info->st_mode)) {
        printf("\tType: block special\n");
        block_num++;
    } else if (S_ISFIFO(file_info->st_mode)) {
        printf("\tType: fifo\n");
        fifo_num++;
    } else if (S_ISLNK(file_info->st_mode)) {
        printf("\tType: symbolic link\n");
        symlink_num++;
    } else if (S_ISSOCK(file_info->st_mode)) {
        printf("\tType: socket\n");
        socket_num++;
    }

    printf("\t Number of links: %lu \n", file_info->st_nlink);
    printf("\t Size: %lu \n", file_info->st_size);
    printf("\t Last access: %lu \n", file_info->st_atime);
    printf("\t Modification time: %lu \n", file_info->st_mtime);
    return 0;
}

void browse_directory_nftw(const char *absolute_path) {
    nftw(absolute_path, (__nftw_func_t) nftw_callback, 2000, FTW_CHDIR | FTW_DEPTH | FTW_PHYS);
    print_cnts();
    reset_cnts();
}
