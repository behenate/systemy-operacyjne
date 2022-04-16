#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int no_arg, char **arg_list) {

    char *consumer[] = {"./consumer","fifo","consumer.txt","5",NULL};
    char *producer1[] = {"./producer","fifo","5","source1.txt","5",NULL};
    pid_t pid_tab[2];
    mkfifo("fifo", 0777);
    if ((pid_tab[0] = fork()) == 0)
        execvp(consumer[0], consumer);

    if ((pid_tab[1] = fork()) == 0) {
        printf("Start producer \n");
        execvp(producer1[0], producer1);
    }

    for (int i = 0; i < 6; i++)
        waitpid(pid_tab[i], NULL, 0);


}