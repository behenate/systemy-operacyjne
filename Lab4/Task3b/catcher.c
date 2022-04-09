#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int signals_cnt = 0;
char * mode;

int s1 = SIGUSR1;
int s2 = SIGUSR2;
int awaiting_response = 0;
sigset_t mask;


void handle_usr1(int sig_no, siginfo_t *info, void *context){
    if (awaiting_response){
        awaiting_response = 0;
        return;
    }else{
        int sender_pid = sender_pid = info->si_pid;
        signals_cnt += 1;
        printf("Catcher Received signal number: %d, replying: \n", signals_cnt);
        kill(sender_pid, s1);
    }

}

void handle_usr2(int sig_no, siginfo_t *info, void *context){
    int sender_pid = info->si_pid;
    printf("\nCatcher Received USR2\n");
    printf("Catcher received %d total USR1 signals \n", signals_cnt);
    printf("Catcher replying %d times \n\n\n", signals_cnt);

    for (int i = 0; i < signals_cnt; ++i) {
        while (awaiting_response)
            sigsuspend(&mask);
        printf("Catcher sending USR1 number: %d \n", i+1);
        ;
        if(strcmp(mode, "KILL") == 0 || strcmp(mode, "SIGRT") == 0){
            kill(sender_pid, s1);
        }else{
            sigval_t to_send;
            to_send.sival_int = signals_cnt;
            sigqueue(sender_pid, s1, to_send);
        }
        awaiting_response = 1;
    }
    while (awaiting_response)
        sigsuspend(&mask);
    kill(sender_pid, s2);
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
        s1 = SIGRTMIN+3;
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


    sigfillset(&mask);
    sigdelset(&mask, s1);
    sigdelset(&mask, s2);
    sigprocmask(SIG_SETMASK, &mask, NULL);
    while (1){
        sigsuspend (&mask);
    }
    return 0;
}