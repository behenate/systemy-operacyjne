// SIGINFO
// SA_RESETHAND
// SA_ONSTACK
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int interrupt_num;

void handle_siginfo(int sig_no, siginfo_t *info, void *context) {
    printf("Got a SIGINT signal\n");
    printf("Signal number: %d \n", sig_no);
    printf("Process ID of sending process: %d \n", info->si_pid);
    printf("Parent PID: %d \n", getppid());
    printf("Signal code: %d\n", info->si_code);
    printf("User time of the signal: %ld\n", info->si_utime);
    printf("Signal value: %d \n\n", info->si_value.sival_int);
}


void test_siginfo() {
    printf("***** TESTING SA_SIGINFO ***** \n");

    struct sigaction act_info;
    act_info.sa_handler = (__sighandler_t) handle_siginfo;
    sigemptyset(&act_info.sa_mask);
    act_info.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &act_info, NULL);
    raise(SIGINT);
    printf("\n\n");
}


void handle_resethand(int sig_no) {
    printf("Got a SIGINT signal with flag SA_RESETHAND \n");
    printf("Resetting signal handling to SIG_DFL \n");
}

void handle_resethand_backup(int sig) {
    printf("Signal handled by the backup handler, SA_RESETHAND worked!");
}


void test_SA_RESETHAND() {
    struct sigaction act_info;
    sigemptyset(&act_info.sa_mask);
    act_info.sa_flags = SA_RESETHAND;
    act_info.sa_handler = (__sighandler_t) handle_resethand;

    sigaction(SIGINT, &act_info, NULL);
    printf("***** TESTING SA_RESETHAND ***** \n");
    printf("Raising with the SA_RESETHAND set: \n");

    raise(SIGINT);
    printf("\nRaising with the SA_RESETHAND set for the second time: \n");

    sleep(1);
//    Setup a backup handler
    signal(SIGINT, handle_resethand_backup);
    raise(SIGINT);
    printf("\n\n");

}

void handle_SA_NODEFER(int sig) {
    int im = interrupt_num;
    interrupt_num++;
    printf("Starting handliing SA_NODEFER number %d \n", im);
    if(interrupt_num < 5)
        kill(getpid(), SIGINT);
    sleep(1);

    printf("Ending handliing SA_NODEFER number %d \n", 5-im-1);

}

void test_SA_NODEFER() {
    /*Flaga oznacza, że kolejne sygnały będą obsługiwane, nawet jeśli aktualnie obsługiwany jest inny sygnał
     * W celu testu należy szybko kliknąć Ctrl+C (obsługa każdego wystąpienia zajmuje 1 sekundę) i zobaczyć, że
     * rzeczywiście kolejne sygnały są obsługiwane, podczas gdy poprzedni nadal jest obsługiwany*/
    struct sigaction act_info;
    printf("***** TESTING SA_NODEFER ***** \n");
    act_info.sa_flags = SA_NODEFER;
    act_info.sa_handler = &handle_SA_NODEFER;
    sigaction(SIGINT, &act_info, NULL);
    raise(SIGINT);
    printf("\n\n");

}


int main(int argc, char **argv) {
    // Test of the sigint
    test_siginfo();
    test_SA_RESETHAND();
    test_SA_NODEFER();
    return 0;
}