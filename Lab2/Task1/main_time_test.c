#include "task1.h"
#include <stdio.h>
#include <sys/times.h>
#include <time.h>
FILE* raport;
void printTimes(struct tms tms_time, clock_t time_real, char * header){
    struct tms tms_time_after;
    clock_t time_real_after = clock();
    times(&tms_time_after);
    double real_time =(double) (time_real_after - time_real) / CLOCKS_PER_SEC;
    double user_time = (double) (tms_time_after.tms_utime - tms_time.tms_utime) / sysconf(_SC_CLK_TCK);
    double system_time =   (double) (tms_time_after.tms_utime - tms_time.tms_utime) / sysconf(_SC_CLK_TCK);
    printf("\n %s \n", header);
    printf("Real time:   %lf     User time: %lf     System time: %lf\n", real_time, user_time,system_time);
    fprintf(raport, "\n %s \n", header);
    fprintf(raport, "Real time:   %lf     User time: %lf     System time: %lf\n", real_time, user_time,system_time);

}


int main(int argc,  char **argv){
    raport = fopen("pomiar_zad_1.txt", "w");
    char in_filename[] = "large.txt";
    char out_filename[] = "large_out.txt";
    struct tms tms_time;
    clock_t time_real;
    times(&tms_time);
    time_real = clock();
    FILE *in_file = fopen(in_filename,"r");
    FILE *out_file = fopen(out_filename, "w");
    copy_remove_whitespace(in_file,out_file);
    printTimes(tms_time, time_real, "TIMES LIB:");
    times(&tms_time);
    time_real = clock();
    int we=open(in_filename, O_RDONLY);
    int wy=open(out_filename,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
    copy_remove_whitespace_sys(we, wy);
    printTimes(tms_time, time_real, "TIMES SYS:");


}