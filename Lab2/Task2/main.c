#include "task2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    char * c = malloc(sizeof (char )*10);
    char * in_filename = calloc(300, sizeof (char));
    if (argc != 3){
        printf("Proszę podać dokładnie dwa pliki 1 - plik wejściowy 2 -"
               " plik wyjściowy (jeśli nie istnieje to program stworzy go automatycznie \n");
        printf("Proszę literęs: \n");
        scanf("%c", c);

        printf("Proszę podać plik wejściowy: \n");
        scanf("%299s", in_filename);

    }else{
        c =  argv[1];
        in_filename = argv[2];
    }
    int mode = 0;
    printf("Proszę numer trybu: 1 - tryb biblioteki, 2 - tryb systemowy: \n >>");
    scanf("%d",&mode);
    if(mode == 1){
        FILE *in_file = fopen(in_filename,"r");
        if (in_file == NULL){
            printf("Podany plik wejściowy nie instnieje!");
            exit(1);
        }
         count(in_file, c[0]);
    }else if(mode == 2){
        int we;
        we=open(in_filename, O_RDONLY);
        count_sys(we, c[0]);
    }else{
        printf("Podany numer trybu nie jest poprawny!");
    }
}