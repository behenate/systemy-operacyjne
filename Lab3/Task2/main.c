#include <stdlib.h>
#include "task2.h"

int main(int argc, char** argv){
    int n;
    double step_size;

//    Remove old files
    char * filename = (char*)malloc(13 * sizeof(char));
    int i = 1;
    system("rm -f w*.txt");
    if(argc == 3){
        step_size = strtod(argv[1], NULL);
        n = atoi(argv[2]);

    } else{
        printf("Step size: ");
        scanf("%lf", &step_size);
        printf("Number of processes: ");
        scanf("%d", &n);

    }

    if (n > 1 / step_size){
        printf("Za mała liczba kwadratów dla podanej ilości procesów!");
        exit(0);
    }
    printf("%lf", calculate_integral(step_size, n));
}