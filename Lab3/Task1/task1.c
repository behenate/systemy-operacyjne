#include <stdlib.h>
#include "task1.h"

void generateProcesses(int n){
    for (int i = 0; i < n; ++i) {
        pid_t child_pid;
        child_pid = fork();
        if(child_pid == 0){
            printf("I am the child process number: %d and my parents pid is: %d \n",i+1, (int)getppid());
            exit(0);
        }
    }
}