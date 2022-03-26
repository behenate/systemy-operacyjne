#include "task3.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char** argv){
    if (argc != 4){
        printf("Please enter the arguments in following format <path> <substring> <max depth>");
        exit(0);
    }
    char * path = argv[1];
    char * substring = argv[2];
    int max_depth = atoi(argv[3]);

    DIR* test = opendir(path);
    printf("\n Searching in directory: %s\n", path);
    browse_directory(test, path, max_depth, argv[2]);
}
