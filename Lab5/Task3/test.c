#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int no_arg, char **arg_list) {

//     wielu producentów, jeden konsument
    char *consumer[] = {"./consumer","fifo","consumer.txt","5",NULL};
    char *producer1[] = {"./producer","fifo","5","source1.txt","5",NULL};
    char *producer2[] = {"./producer","fifo","4","source2.txt","5",NULL};
    char *producer3[] = {"./producer","fifo","3","source3.txt","5",NULL};
    char *producer4[] = {"./producer","fifo","1","source3.txt","5",NULL};
    char *producer5[] = {"./producer","fifo","2","source4.txt","5",NULL};
    pid_t pid_tab[6];
    mkfifo("fifo", 0777);
    if ((pid_tab[0] = fork()) == 0)
        execvp(consumer[0], consumer);

    if ((pid_tab[1] = fork()) == 0) {
        printf("Start producer \n");
        execvp(producer1[0], producer1);
    }
    if ((pid_tab[2] = fork()) == 0) {
        printf("Start producer \n");
        execvp(producer2[0], producer2);
    }
    if ((pid_tab[3] = fork()) == 0){
        printf("Start producer \n");
        execvp(producer3[0], producer3);
    }
    if ((pid_tab[4] = fork()) == 0){
        printf("Start producer \n");
        execvp(producer4[0], producer4);
    }
    if ((pid_tab[5] = fork()) == 0){
        printf("Start producer \n");
        execvp(producer5[0], producer5);
    }

    for (int i = 0; i < 6; i++)
        waitpid(pid_tab[i], NULL, 0);

    // Wielu konsumentów jeden producent N = 30
    char *consumera1[] = {"./consumer", "fifo", "consumera.txt", "1", NULL};
    char *consumera2[] = {"./consumer", "fifo", "consumera.txt", "1", NULL};
    char *consumera3[] = {"./consumer", "fifo", "consumera.txt", "1", NULL};

    char *producera1[] = {"./producer", "fifo", "30", "source1.txt", "1", NULL};

    pid_t pid_taba[4];
    mkfifo("fifo", 0666);
    if ((pid_taba[0] = fork()) == 0)
        execvp(consumera1[0], consumera1);

    if ((pid_taba[1] = fork()) == 0) {
        execvp(consumera2[0], consumera2);
    }
    if ((pid_taba[2] = fork()) == 0) {
        execvp(consumera3[0], consumera3);
    }
    if ((pid_taba[3] = fork()) == 0) {
        execvp(producera1[0], producera1);
    }

    for (int i = 0; i < 4; i++)
        waitpid(pid_taba[i], NULL, 0);

// Wielu konsumentów jeden producent N = 30
    char *consumerb1[] = {"./consumer", "fifo", "consumerb.txt", "4600", NULL};
    char *consumerb2[] = {"./consumer", "fifo1", "consumerb1.txt", "4600", NULL};
    char *consumerb3[] = {"./consumer", "fifo2", "consumerb2.txt", "4600", NULL};

    char *producerb1[] = {"./producer", "fifo", "1", "source1.txt", "4600", NULL};
    char *producerb2[] = {"./producer", "fifo1", "2", "source2.txt", "4600", NULL};
    char *producerb3[] = {"./producer", "fifo2", "3", "source3.txt", "4600", NULL};



    pid_t pid_tabb[6];
    mkfifo("fifo", 0666);
    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    if ((pid_tabb[0] = fork()) == 0)
        execvp(consumerb1[0], consumerb1);

    if ((pid_tabb[1] = fork()) == 0) {
        execvp(consumerb2[0], consumerb2);
    }
    if ((pid_tabb[2] = fork()) == 0) {
        execvp(consumerb3[0], consumerb3);
    }
    if ((pid_tabb[3] = fork()) == 0) {
        execvp(producerb1[0], producerb1);
    }
    if ((pid_tabb[4] = fork()) == 0) {
        execvp(producerb2[0], producerb2);
    }
    if ((pid_tabb[5] = fork()) == 0) {
        execvp(producerb3[0], producerb3);
    }

    for (int i = 0; i < 6; i++)
        waitpid(pid_taba[i], NULL, 0);

}