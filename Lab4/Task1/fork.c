#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>


void ignore_checker() {
    printf("Making the parent process ignore the signal!\n");
    signal(SIGUSR1, SIG_IGN);

    raise(SIGUSR1);
    printf("Raised the signal in the parent!\n");


    pid_t pid = fork();
    if (pid == 0){
        raise(SIGUSR1);
        printf("Raised the signal in the child process!\n");
    }
    else {
        wait(NULL);
    }
}
void handle(){
    printf("Handled the signal! from PID: %d, parent: %d \n", getpid(), getppid());
}
void handler_checker(){
    printf("Making the parent process handle the signal!\n");
    signal(SIGUSR1, handle);

    printf("Raised the signal in the parent!\n");
    raise(SIGUSR1);

    pid_t pid = fork();
    if (pid == 0){
        printf("Raised the signal in the child process!\n");
        raise(SIGUSR1);
    }
    else {
        wait(NULL);
    }
}

void mask_pending_checker(int is_mask){
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    printf("Adding the signal to the mask!\n");
    sigprocmask(SIG_SETMASK, &mask, NULL);
    raise(SIGUSR1);
    printf("Raised the singnal in the parent!\n");

    printf("Testing pending from parent: ");
    sigpending(&mask);
    if(sigismember(&mask, SIGUSR1)){
        printf("The signal is pending \n");
    }else{
        printf("The signal is not pending \n");
    }
    pid_t pid = fork();
    if (pid == 0){
        if (is_mask){
            raise(SIGUSR1);
            printf("Raising the signal in the child process! \n");
        }
        sigpending(&mask);
        if(sigismember(&mask, SIGUSR1)){
            printf("The signal is pending in the child \n");
        }else{
            printf("The signal is not pending in the child \n");
        }
    }
    else {
        wait(NULL);
    }
}


int main(int argc, char** argv){
    if(argc > 2){
        printf("Please enter a valid number of arguments");
        exit(1);
    }
    if(argc == 1){
        printf("Enter mode: ");
        char * buf = malloc(1000);
        scanf("%s", buf);
        argv[1] = buf;
    }
    if(strcmp(argv[1], "ignore") == 0){
        printf("Testing ignore: \n");
        ignore_checker();
    }else if(strcmp(argv[1], "handler") == 0){
        printf("Testing handler: \n");
        handler_checker();
    }else if (strcmp(argv[1],"mask")==0){
        printf("Testing mask: \n");
        mask_pending_checker(1);
    }else if (strcmp(argv[1], "pending")==0){
        printf("Testing pending: \n");
        mask_pending_checker(0);
    } else{
        printf("Passed argument is invalid");
    }
    return 0;
}
