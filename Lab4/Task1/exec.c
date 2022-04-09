#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void ignore_checker() {
    printf("Making the parent process ignore the signal!\n");
    signal(SIGUSR1, SIG_IGN);

    printf("Raising the signal in the parent!\n");
    raise(SIGUSR1);
    execl("./subprocess", "./subprocess","ignore", NULL);
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

    if (is_mask){
        execl("./subprocess", "./subprocess","mask", NULL);

    }else{
        execl("./subprocess", "./subprocess","pending", NULL);
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
        ignore_checker();
    }else if (strcmp(argv[1],"mask")==0){
        mask_pending_checker(1);
    }else if (strcmp(argv[1], "pending")==0){
        mask_pending_checker(0);
    } else{
        printf("Passed argument is invalid");
    }
    return 0;
}