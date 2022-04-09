#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
    char total_signals[100];
    char mode[100];

    printf("Please enter the total number of signals: \n >>");
    scanf("%s", total_signals);
    printf("\n Please enter the mode (KILL/SIGQUEUE/SIGRT): \n >>");
    scanf("%s", mode);
    printf("STARTING! \n");

    int catcher_pid = fork();
    if (catcher_pid == 0){
        printf("Starting catcher main\n");
        execl("./catcher", "./catcher", mode, NULL);
    }else{
        int sender_pid = fork();
        if(sender_pid == 0){
            char pid_str[100];
            sprintf(pid_str,"%d", catcher_pid);
            execl("./sender", "./sender", pid_str,total_signals, mode, NULL);
        } else{
            sleep(2);

            for (int i = 0; i < 3; ++i) {
                printf("Time left: %d \n", (3-i));
                sleep(1);
            }
            printf("Time left: 0 \n");
        }
    }
    return 0;
}


