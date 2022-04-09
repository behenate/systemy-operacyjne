#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "signal.h"

int target_signals = 0;
int signals_cnt = 0;
int expected_signals = 0;
int awaiting_response = 0;

char *mode;
int s1 = SIGUSR1;
int s2 = SIGUSR2;
void handle_usr1(int sig_no, siginfo_t *info, void *context) {
    if (awaiting_response){
        awaiting_response = 0;
        return;
    }else{
        signals_cnt += 1;
        if (strcmp(mode, "SIGQUEUE") == 0) {
            expected_signals = info->si_value.sival_int;
            printf("Sender expecting %d USR signals. ", expected_signals);
        }
        printf("Sender received USR1 number %d \n", signals_cnt);
        kill(info->si_pid, s1);
    }

}

void handle_usr2(int sig_no, siginfo_t *info, void *context) {
    printf("\nSender received USR2 \n");
    if (strcmp(mode, "SIGQUEUE") == 0) {
        printf("\nSender should have received %d signals, actually received: %d signals, the catcher has sent %d signals \n",
               target_signals, signals_cnt, expected_signals);
    } else {
        printf("\nSender should have received %d signals, actually received: %d signals \n", target_signals,
               signals_cnt);
    }
    printf("\nSender ending its work, see ya \n");
    exit(0);
}

int main(int argc, char **argv) {

    int catcher_pid = atoi(argv[1]);
    target_signals = atoi(argv[2]);
    mode = argv[3];

    if (strcmp(mode, "SIGRT") == 0) {
        s1 = SIGRTMIN + 3;
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

    printf("Initializing the Sender \n");
    sleep(1);

    if (target_signals < 50) {
        printf("Not enough signals, please enter at least 50, exiting \n");
        exit(0);
    }

    for (int i = 0; i < target_signals; ++i) {
        printf("Sender sending USR1 number: %d \n", i+1);
        kill(catcher_pid, s1);
        awaiting_response=1;
        while (awaiting_response)
            sigsuspend(&mask);
    }
    kill(catcher_pid, s2);


    printf("Sender turning on receiver mode: \n");
    while (1)
        sigsuspend(&mask);

    return 0;
}