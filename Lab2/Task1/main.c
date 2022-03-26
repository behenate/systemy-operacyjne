#include "task1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,  char **argv){
    char * in_filename = calloc(300, sizeof (char));
    char * out_filename = calloc(300, sizeof (char));
    if (argc != 3){
        printf("Proszę podać dokładnie dwa pliki 1 - plik wejściowy 2 -"
               " plik wyjściowy (jeśli nie istnieje to program stworzy go automatycznie \n");
        printf("Proszę podać plik wejściowy: \n");
        scanf("%299s", in_filename);
        printf("Proszę podać plik wyjściowy: \n");
        scanf("%299s", out_filename);
    }else{
        in_filename =  argv[1];
        out_filename = argv[2];
    }
    int mode = 0;
    printf("Proszę numer trybu: 1 - tryb biblioteki, 2 - tryb systemowy: \n >>");
    scanf("%d",&mode);
    if(mode == 1){
        FILE *in_file = fopen(in_filename,"r");
        FILE *out_file = fopen(out_filename, "w");
        if (in_file == NULL){
            printf("Podany plik wejściowy nie instnieje!");
            exit(1);
        }
        copy_remove_whitespace(in_file, out_file);
    }else if(mode == 2){
        int we,wy;
        we=open(in_filename, O_RDONLY);
        wy=open(out_filename,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
        copy_remove_whitespace_sys(we, wy);
    }
}