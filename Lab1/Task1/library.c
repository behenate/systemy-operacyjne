#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **pointers_arr;
char * buffer = 0;
int *free_indices;
unsigned int pointers_len = 0;

// Alokuje tablicę bloków o zadanym rozmiarze
void create_table(unsigned int n){
    pointers_arr = (char **) calloc(n, sizeof( char *));
    free_indices = (int *) calloc(n, sizeof (int));
    pointers_len = n;
}

// Zczytuję wyniki polecenia wc do pliku tymczasowego
void do_wc(char* strings[], int num_files){
    FILE *tmpfile = fopen("tmpfile", "a");
//    Dla każdego z podanych plików zapisuję wyniki do pliku tymczasowego
    for (int i =0; i< num_files; i++){
        FILE *in_file;
        in_file = fopen(strings[i], "r"); // read only
        FILE *fp;

        char command[100] = "wc ";
        strcat(command, strings[i]);
        fp = popen(command,"r");

        wcResult result;
        fscanf(fp,"%d %d %d", &result.lines, &result.words, &result.characters);
        fprintf(tmpfile, "%d %d %d \n", result.lines, result.words, result.characters);
        fclose(in_file);
    }
    fclose(tmpfile);
}

// Funkcja znajdująca wolne pole w tablicy blokóœ
int find_free_index(){
    for (int i =0; i < pointers_len; i++){
        if(free_indices[i]==0){
            return i;
        }
    }
    return -1;
}

// Funkcja zapisująca zawartość pliku tmpfile do wolnego bloku
int save_to_memory(){
    FILE *tmp_file = fopen("tmpfile", "r");
    if(!tmp_file){
        printf("No operations to save!");
        return -1;
    }
//    Znajdowanie rozmiaru pliku tmpfile i alokacja bufora
    fseek(tmp_file, 0L, SEEK_END);
    int file_size = ftell(tmp_file);
    if(file_size == 0){
        printf("No operations to save!");
        return -1;
    }
    fseek(tmp_file, 0, SEEK_SET);
    buffer = calloc (file_size, sizeof (char));
    if (buffer){
        fread (buffer, 1, file_size, tmp_file);
    }
    int free_index = find_free_index();
//    Jeśli jest miejsce w tablicy, zapisanie
    if (free_index != -1 && free_index < pointers_len){
        pointers_arr[free_index] = buffer;
        free_indices[free_index] = 1;
        remove("tmpfile");
        return free_index;
    }
    printf("No place left in the memory blocks!");
    return -1;
}

// Zwalnia blok o podanym indeksie
void delete_block(int idx)
{
    if(pointers_arr[idx] != NULL){
        free(pointers_arr[idx]);
        pointers_arr[idx]=NULL;
        free_indices[idx]=0;
    }
}