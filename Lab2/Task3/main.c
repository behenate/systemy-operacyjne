#include "task3.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>


int main(int argc, char** argv){
    if(argc != 2){
        printf("Proszę o podanie poprawnej ścieżki!");
        exit(1);
    }
    char abs_path[3000];

    realpath(argv[1], abs_path);
//    printf("%s", abs_path);
    DIR* test = opendir(abs_path);
    browse_directory(test, abs_path);
    print_cnts();
    reset_cnts();
//    browse_directory_nftw(abs_path);
}