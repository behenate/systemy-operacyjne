#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef SYSOPY_TASK2_H
#define SYSOPY_TASK2_H

void count(FILE *file, char c);
int count_char(char *line, int length, char c);
void count_sys(int file, char c);
#endif //SYSOPY_TASK2_H
