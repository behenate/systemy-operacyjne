#include "task2.h"
#include <stdio.h>
#include <sys/times.h>
#include <time.h>
#include <sys/wait.h>

FILE* raport;
pid_t wpid;
void printTimes(struct tms start_tms_time, clock_t time_real,struct tms tms_time_after,clock_t time_real_after,
        char * header){
    double real_time =(double) (time_real_after - time_real) / CLOCKS_PER_SEC;
    double user_time = (double) (tms_time_after.tms_utime - start_tms_time.tms_utime) / sysconf(_SC_CLK_TCK);
    double system_time = (double) (tms_time_after.tms_utime - start_tms_time.tms_utime) / sysconf(_SC_CLK_TCK);
    fprintf(raport, "%d %d %d\n", getpid(), getppid(), wpid);
    printf("TESTING");
    printf("\n %s \n", header);
    printf("Real time:   %lf     User time: %lf     System time: %lf\n", real_time, user_time,system_time);
    fprintf(raport, "\n %s \n", header);
    fprintf(raport, "Real time:   %lf     User time: %lf     System time: %lf\n", real_time, user_time,system_time);
}


int main(int argc,  char **argv){
    wpid = getpid();

    raport = fopen("pomiar_zad_2.txt", "w");
    struct tms tms_times[7];
    clock_t times_real[7];

//    Save start time
    times(&tms_times[0]);
    times_real[0] = clock();

    calculate_integral(0.000000000000001, 10000);
//    Save start/end time
    times(&tms_times[1]);
    times_real[1] = clock();

    calculate_integral(0.000000000000001, 10);
//    And so on...
    times(&tms_times[2]);
    times_real[2] = clock();

    calculate_integral(0.000000000000001, 1);

    times(&tms_times[3]);
    times_real[3] = clock();

    calculate_integral(0.0000000000001, 100);

    times(&tms_times[4]);
    times_real[4] = clock();

    calculate_integral(0.00000001, 100);

    times(&tms_times[5]);
    times_real[5] = clock();

    calculate_integral(0.00000001, 1);
//    Save end time of last execution
    times(&tms_times[6]);
    times_real[6] = clock();

//    Print the times
    printTimes(tms_times[0], times_real[0], tms_times[1], times_real[1],
               "Testing square width: 0.000000000000001, processes num : 10000\n");
    printTimes(tms_times[1], times_real[1], tms_times[2], times_real[2],
               "Testing square width: 0.000000000000001, processes num : 10\n");
    printTimes(tms_times[2], times_real[2], tms_times[3], times_real[3],
               "Testing square width: 0.000000000000001, processes num : 1\n");
    printTimes(tms_times[3], times_real[3], tms_times[4], times_real[4],
               "Testing square width: 0.0000000000001, processes num : 100\n");
    printTimes(tms_times[4], times_real[4], tms_times[5], times_real[5],
               "Testing square width: 0.00000001, processes num : 100\n");
    printTimes(tms_times[5], times_real[5], tms_times[6], times_real[6],
               "Testing square width: 0.00000001, processes num : 1\n");







}