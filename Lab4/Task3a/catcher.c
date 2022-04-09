#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int sender_pid = 0;
int signals_cnt = 0;
char * mode;

int s1 = SIGUSR1;
int s2 = SIGUSR2;

void handle_usr1(int sig_no, siginfo_t *info, void *context){
    signals_cnt += 1;
    if (sender_pid == 0){
        sender_pid = info->si_pid;
        printf("\nCatcher Received the initial signal, sender PID: %d \n", sender_pid);

    }else
        printf("Catcher Received signal number: %d \n", signals_cnt);
}
void handle_usr2(int sig_no, siginfo_t *info, void *context){
    printf("\nCatcher Received USR2\n");
    printf("Catcher received %d total USR1 signals \n", signals_cnt);
    printf("Catcher replying %d times \n\n\n", signals_cnt);

    if(strcmp(mode, "KILL") == 0 || strcmp(mode, "SIGRT") == 0){
        for (int i = 0; i < signals_cnt; ++i) {
            printf("Catcher sending USR1 number: %d \n", i+1);
            kill(sender_pid, s1);
        }
        kill(sender_pid, s2);
    }else{
        for (int i = 0; i < signals_cnt; ++i) {
            sigval_t to_send;

            printf("Catcher sending USR1 number: %d \n", i+1);
            to_send.sival_int = signals_cnt;
            sigqueue(sender_pid, s1, to_send);
        }
        kill(sender_pid, s2);
    }


    printf("Catcher done, see ya \n");
    exit(0);
}
int main(int argc, char **argv) {
    printf("Starting catcher, Catcher PID: %d \n", getpid());
    if (argc == 1){
        printf("Catcher should have received one of the following modes KILL/SIGQUEUE/SIGRT please provide one of them \n");
        printf("like: ./catcher KILL \n exiting\n");
        exit(0);
    }
    mode = argv[1];


    if (strcmp(mode, "SIGRT") == 0) {
        s1 = SIGRTMIN + 1;
        s2 = SIGRTMIN + 2;
    }

    struct sigaction act_usr1;
    act_usr1.sa_flags = SA_SIGINFO;
    act_usr1.sa_handler = (__sighandler_t) &handle_usr1;
    sigaction(s1, &act_usr1, NULL);

    struct sigaction act_usr2;
    act_usr2.sa_flags = SA_SIGINFO;
    act_usr2.sa_handler = (__sighandler_t) &handle_usr2;
    sigaction(s2, &act_usr2, NULL);
    sigset_t mask;


    sigfillset(&mask);
    sigdelset(&mask, s1);
    sigdelset(&mask, s2);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    while (1){
        sigsuspend (&mask);
    }
    return 0;
}