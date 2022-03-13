#ifndef LIBRARY_H
#define LIBRARY_h



void create_table(unsigned int n);
typedef struct  WcResult {
    int characters;
    int words;
    int lines;
} wcResult;

void do_wc(char *strings[], int numFiles);
int save_to_memory();
void delete_block(int idx);
#endif