#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <sys/file.h>


int main(int argc, char **argv) {
    printf("From producer \n");
    char *stream_path = calloc(1000, sizeof(char));
    char *file_path = calloc(1000, sizeof(char));
    int n = 0;
    char *row;

    if (argc == 5) {
        stream_path = argv[1];
        row = argv[2];
        file_path = argv[3];
        n = atoi(argv[4]);
    }
    printf("From producer %s %s\n", stream_path, file_path);


    FILE* stream = NULL;
    stream = fopen(stream_path, "w");

    FILE *file = NULL;
    char buffer[n];
    size_t bytesRead = 0;
    file = fopen(file_path, "r");

    // read up to sizeof(buffer) bytes
    while ((bytesRead = fread(buffer, sizeof (char ), n, file)) > 0)
    {
        flock(fileno(file), LOCK_EX);
        char * line_no = calloc(5, sizeof (char ));
        sprintf(line_no, "%d", 90000+ atoi(row));
        fwrite(line_no, sizeof (char ), 5, stream);
        fwrite(buffer, sizeof (char), n, stream);
//        sleep(1);
        usleep(50000);
        flock(fileno(file), LOCK_UN);
    }
    fclose(stream);
    fclose(file);


}