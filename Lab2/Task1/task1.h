//
// Created by wojciech on 17.03.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


#ifndef LAB1_TASK1_H
#define LAB1_TASK1_H


void copy_remove_whitespace(FILE *file, FILE *target_file);
int is_whitespaces(char *line, int length);
void copy_remove_whitespace_sys(int file, int target_file);


#endif //LAB1_TASK1_H
