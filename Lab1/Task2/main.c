#include <stdio.h>
#include "../Task1/library.h"
#include <string.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>

FILE *raport;


// Funkcja sprawdzająca czy podane słowo jest komendą i zwracająca identyfikator
int command_idx(char *command) {
    char *commands[] = {"create_table", "wc_files", "save_to_memory", "remove_block", "save_remove"};
    for (int i = 0; i < 5; ++i) {
        if (strcmp(commands[i], command) == 0) {
            return i;
        }
    }
    return -1;
}

// Funkcja na przemian dodająca i usuwająca kilka bloków podaną liczbę razy
void save_remove(int num_blocks){
    FILE *tmpFile;
    tmpFile = fopen("blockTest3", "w");

    fprintf(tmpFile, "%s", "asdfsdfsd asdfkjasdlkf jlsadkfj lksadjflksdajf lkjsdaf lkjdasl fkj\n asdf asdf");
    fclose(tmpFile);

    for (int k = 0; k < 16; ++k) {
        int savedBlocks[num_blocks];
//        Dodaj bloki
        for (int j = 0; j < num_blocks; ++j) {
            char *test[] = {"blockTest3"};
            do_wc(test, 1);
            int idx = save_to_memory();
            savedBlocks[j] = idx;
            if(idx==-1){
                printf("No space in the memory for the operation!");
                exit(1);
            }
        }
//        Usuń bloki
        for (int j = 0; j < num_blocks; ++j) {
            delete_block(savedBlocks[j]);
        }
    }
    remove("blockTest3");
}


// Funkcja wypisująca czas operacji od momentu wywołania
void print_times(struct tms tms_time, clock_t time_real ){
    struct tms tms_time_after;
    clock_t time_real_after = clock();
    times(&tms_time_after);
    printf("Real time:   %lf \n", (double) (time_real_after - time_real) / CLOCKS_PER_SEC);
    printf("User time:   %lf \n",
           (double) (tms_time_after.tms_utime - tms_time.tms_utime) / sysconf(_SC_CLK_TCK));
    printf("System time: %lf \n",
           (double) (tms_time_after.tms_utime - tms_time.tms_utime) / sysconf(_SC_CLK_TCK));

    fprintf(raport ,"Real time:   %lf \n", (double) (time_real_after - time_real) / CLOCKS_PER_SEC);
    fprintf(raport, "User time:   %lf \n",
            (double) (tms_time_after.tms_utime - tms_time.tms_utime) / sysconf(_SC_CLK_TCK));
    fprintf(raport, "System time: %lf \n \n",
            (double) (tms_time_after.tms_utime - tms_time.tms_utime) / sysconf(_SC_CLK_TCK));

}

// Funkcja testująca czas alokacji
void test_allocation(int size) {
    struct tms tms_time;
    clock_t time_real;
    printf("-------TABLE OF SIZE %d ALLOCATION TEST ------- \n", size);
    fprintf(raport,"-------TABLE OF SIZE %d ALLOCATION TEST------- \n", size);

    times(&tms_time);
    time_real = clock();
    create_table(size);
    print_times(tms_time, time_real);

}

// Funkcja testująca czas wykonania polecania wc i zapisania do pliku
void test_read(char*files[], int num_files, char* header) {
    struct tms tms_time;
    clock_t time_real;
    printf(header, files);
    fprintf(raport,"%s", header);

    times(&tms_time);
    time_real = clock();
    do_wc(files,num_files);
    print_times(tms_time, time_real);

}

// Funkcja testująca czas zapisu do bloku
void test_write(char* desc){
    struct tms tms_time;
    clock_t time_real;
    printf("------- TEST SAVE TO BLOCK %s ------- \n", desc);
    fprintf(raport, "------- TEST SAVE TO BLOCK %s ------- \n", desc);

    times(&tms_time);
    time_real = clock();
    save_to_memory();
    print_times(tms_time, time_real);
}

// Funkcja testująca czas usuwania bloków
void test_remove(int num_blocks, char* avail_files[], int num_files){
    struct tms tms_time;
    clock_t time_real;

    for (int i = 0; i < num_blocks; ++i) {
        char * file[] = {avail_files[i%num_files]};
        do_wc(file,1);
        save_to_memory();
    }

    printf("------- TEST REMOVE BLOCKS, NO. BLOCKS: %d ------- \n", num_blocks);
    fprintf(raport, "------- TEST TO REMOVE, NO. BLOCKS %d ------- \n", num_blocks);
    times(&tms_time);
    time_real = clock();
    for (int i = 0; i < num_blocks; ++i) {
        delete_block(i);
    }

    print_times(tms_time, time_real);

}

// Funkcja testująca czas naprzemiennego usuwania i dodawania bloków
void test_save_remove(int num_blocks){
    struct tms tms_time;
    clock_t time_real;
    printf("------- TEST SAVE AND REMOVE BLOCKS, NO. BLOCKS: %d ------- \n", num_blocks);
    fprintf(raport, "------- TEST SAVE AND REMOVE BLOCKS, NO. BLOCKS %d ------- \n", num_blocks);
    times(&tms_time);
    time_real = clock();
    save_remove(num_blocks);
    print_times(tms_time, time_real);
}

// Funkcja generująca raport z testóœ
void test() {
    char* small = {"../Task1/small.txt"};
    char* medium = {"../Task1/medium.txt"};
    char* large = {"../Task1/large.txt"};
    raport = fopen("../Task2/raport2.txt", "w");
    test_allocation(1000);

    char * read1_files[] = {small, medium, large};
    test_read(read1_files,3,  "------- TEST WC ON Files small, medium, large ------- \n");

    char * read2_files[] = {small, medium, large,large,large,large, medium};
    test_read(read2_files,7,  "------- TEST WC ON Files small.txt, medium.txt, large.txt, large.txt,large.txt,large.txt, medium.txt ------- \n");

    char * read3_files[] = {small};
    test_read(read3_files,1,  "------- TEST WC ON Files small.txt ------- \n");

    char * read4_files[] = {small, small, small,medium,medium,small, small};

    test_write("THREE BLOCKS");
    for (int i = 0; i < 10; ++i) {
        do_wc(read4_files, 7);
    }
    test_write("BIG BLOCK");
    test_remove(100, read1_files, 3);
    test_save_remove(10);
    fclose(raport);
}


// Funkcja parsująca komendy użytkownika
int main(int argc, char *argv[]) {
    if(argc == 1){
        test();
    }
    for (int i = 1; i < argc; ++i) {
        int cmd_idx = command_idx(argv[i]);
        if (cmd_idx == 0) { // Alokacja tablicy
            i++;
            int table_size = atoi(argv[i]);
            if (i>=argc || table_size <=0){
                printf("Please enter a valid table size!");
                exit(1);
            }
            create_table(table_size);
        } else if (cmd_idx == 1) { //Wykonanie polecenia wc
            i++;
            if (i >= argc || command_idx(argv[i]) != -1){
                printf("Provide valid files for the command!");
                exit(1);
            }

            int files_count = 0;
            while (i<argc && command_idx(argv[i]) == -1) {
                files_count++;
                i++;
            }
            i -= files_count;
            char **filenames = calloc(files_count, (sizeof(char)) * 100);
            for (int j = 0; j < files_count; ++j) {
                filenames[j] = argv[i + j];
            }
            do_wc(filenames, files_count);
        } else if (cmd_idx == 2) { // Zapisanie pliku tymczasowego do bloku pamięci
            save_to_memory();
        } else if (cmd_idx == 3) { // Usunięcie danych z bloku pamięci
            i++;
            if(i >= argc){
                printf("Please provide a delete index");
                exit(1);
            }
            int delete_index = atoi(argv[i]);
            delete_block(delete_index);
        } else if (cmd_idx == 4) { // Wykonanie operacju usuwania i dodawania kilku bloków podana liczbę razy
            i++;
            if(i >= argc){
                printf("Please provide a valid number of blocks to save and remove");
                exit(1);
            }
            int num_blocks = atoi(argv[i]);
            if(num_blocks <=0){
                printf("Please provide a valid number of blocks to save and remove");
                exit(1);
            }
            save_remove(num_blocks);
        }
    }
}
