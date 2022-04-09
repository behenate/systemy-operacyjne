#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void handle(){
    printf("Handled the signal! from PID: %d, parent: %d \n", getpid(), getppid());
}

int main(int argc, char** argv) {
    printf("This is the child launched from exec \n");
    if(strcmp(argv[1], "ignore") == 0){
        raise(SIGUSR1);
        printf("Raised the signal in the child!\n");
    }else if (strcmp(argv[1],"mask")==0 || strcmp(argv[1],"pending")==0){
        sigset_t mask;
        sigemptyset(&mask);
        if(strcmp(argv[1],"mask")==0){
            raise(SIGUSR1);
            printf("Raising the signal in the child process! \n");
        }
        printf("Testing pending in the child: ");
        sigpending(&mask);
        if(sigismember(&mask, SIGUSR1)){
            printf("The signal is pending \n");
        }else{
            printf("The signal is not pending \n");
        }
    }
    return 0;
}