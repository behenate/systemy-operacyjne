#include <stdio.h>
# include "library.h"
int main(){
    remove("tmpfile");
    create_table(2);
    char* a2[] = { "test.txt", "test2.txt" };
    do_wc(a2, 2);
    do_wc(a2, 2);
    do_wc(a2, 2);

    save_to_memory();
    do_wc(a2, 2);
    save_to_memory();

    delete_block(0);
    do_wc(a2, 2);
    save_to_memory();

    do_wc(a2, 2);
    do_wc(a2, 2);

}