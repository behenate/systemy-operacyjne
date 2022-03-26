#include <stdlib.h>
#include "task1.h"

int main(int argc, char** argv){
    int n;
    if(argc == 2){
        n = atoi(argv[1]);
    } else{
        printf("Number of processes: ");
        scanf("%d", &n);
    }
    generateProcesses(n);
}