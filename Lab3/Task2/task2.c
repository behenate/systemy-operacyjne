#include <sys/wait.h>
#include "task2.h"
double calculate_integral_part(double startPart, int n_squares, double width){
    double res = 0;
    for (int i = 0; i < n_squares; ++i) {
        double start = (startPart + (i)*width);
        double center = start + (width)/2;
        double value = 4 / (pow(center, 2) + 1);
        res += value * (width);
    }
    return res;
}

double calculate_integral(double width, int n){
    int steps = (int)(1.0/width);
    int steps_per_iteration = steps/n;
    int remainder = steps%n;


    for (int i = 0; i < n; ++i) {
        pid_t child_pid;
        child_pid = fork();
        if(child_pid == 0){

            int done_steps = i * steps_per_iteration;
            int add_remainder = 0;
            if(i<=remainder-1){
                done_steps += i;
                add_remainder = 1;
            } else{
                done_steps += remainder;
            }

            double child_res = calculate_integral_part(done_steps*width, steps_per_iteration+add_remainder, width);
            char *filename = (char*)malloc(13 * sizeof(char));
            sprintf(filename, "w%d.txt", i+1);
            FILE* file = fopen(filename, "w");
            fprintf(file, "%f", child_res);
            exit(0);
        }else{
            wait(NULL);
        }
    }
//    Wait for the children to finish
    pid_t wpid;
    int status = 0;
    while ((wpid = wait(&status)) > 0);


    double solution = 0 ;
    for (int i = 0; i < n; ++i) {
        char *filename = (char*)malloc(13 * sizeof(char));
        sprintf(filename, "w%d.txt", i+1);
        FILE* file = fopen(filename, "r");
        fseek (file, 0, SEEK_END);
        long length = ftell (file);
        fseek (file, 0, SEEK_SET);
        char * buffer = malloc (length);
        if (buffer)
        {
            fread (buffer, 1, length, file);
        }
        fclose (file);
        char *eptr;
        solution += strtod(buffer, &eptr);
    }
    return solution;

}

