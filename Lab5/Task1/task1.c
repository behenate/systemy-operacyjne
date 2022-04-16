#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define MAX_COMMANDS_CNT 100
#define MAX_COMMAND_LEN 1000
#define MAX_ARGS_CNT 100
char **commands;


void execute_commands(char * split_commands[MAX_COMMANDS_CNT][MAX_ARGS_CNT]) {
    int commands_cnt = 0;
    while (split_commands[commands_cnt][0] != NULL)
        commands_cnt++;
//    for (int i = 0; i < commands_cnt; ++i) {
//        printf("%s %s %s \n", split_commands[i][0], split_commands[i][1], split_commands[i][2]);
//    }

//    Create a buffer for the pipes
    int fds[commands_cnt-1][2];
    for (int i = 0; i < commands_cnt-1; ++i) {
        pipe(fds[i]);
    }
//Run all of the commands
    for (int i = 0; i < commands_cnt; ++i) {
        if(fork() == 0){
            if(i-1 >=0)
                dup2(fds[i-1][0], STDIN_FILENO);
            if(i != commands_cnt-1)
                dup2(fds[i][1],STDOUT_FILENO);
            for (int j = 0; j < commands_cnt-1; ++j) {
                    close(fds[j][1]);
                    close(fds[j][0]);
            }
            execvp(split_commands[i][0], split_commands[i]);
            exit(0);
        }

    }
    for (int j = 0; j < commands_cnt-1; ++j) {
        close(fds[j][1]);
        wait(0);
    }

}


int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        printf("Podaj poprawną liczbę argumentów!");
        exit(1);
    }
    commands = calloc(MAX_COMMANDS_CNT, sizeof(char *));
    for (int i = 0; i < MAX_COMMANDS_CNT; ++i) {
        commands[i] = calloc(MAX_COMMAND_LEN, sizeof(char));
    }
    char *filename = malloc(1000);
    filename = argv[1];

    FILE *commands_list = fopen(filename, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;


    int commands_cnt = 0;
    while ((read = getline(&line, &len, commands_list)) != -1) {
        int spaces_cnt = 0;
        int start_index = 0;
        while (spaces_cnt < 2) {
            if (line[start_index] == ' ')
                spaces_cnt++;
            start_index++;
        }
        for (int i = start_index; i < MAX_COMMAND_LEN; ++i) {
            if(line[i] == '\n')
                break;
            commands[commands_cnt][i - start_index] = line[i];
        }
        commands_cnt++;
    }

    fclose(commands_list);
    if (line)
        free(line);


    char *combination = malloc(10000);
    if (argc != 3) {
        printf("Proszę podaj komendę w postaci składnik1 | składnik2 | ... | składnikn \n");
        scanf("%[^\n]", combination);
    } else
        combination = argv[2];

    char delim[] = " ";
    char *ptr = strtok(combination, delim);
    int tasks_cnt = 0;
    char *split_commands[MAX_COMMANDS_CNT][MAX_ARGS_CNT];
    for (int i = 0; i < MAX_COMMANDS_CNT; ++i) {
        for (int j = 0; j < MAX_ARGS_CNT; ++j) {
            split_commands[i][j] = NULL;
        }
    }
    int row = 0;
    while (ptr != NULL) {
        if (strcmp(ptr, "|") != 0) {
            int num = 0;
            sscanf(ptr, "%*[^0123456789]%d", &num);
            char delim1[] = "|";
            char *token1;
            char * commands_copy = calloc(MAX_COMMAND_LEN, sizeof (char ));
            strcpy(commands_copy, commands[num-1]);
            char *ptr1 = strtok_r(commands_copy, delim1, &token1);
            while (ptr1 != NULL) {
                char *token2;
                char *delim2 = " ";
                char *ptr2 = strtok_r(ptr1, delim2, &token2);
                int col = 0;
                while (ptr2 != NULL) {
                    split_commands[row][col] = ptr2;
                    ptr2 = strtok_r(NULL, delim2, &token2);
                    col++;
                }
                ptr1 = strtok_r(NULL, delim1, &token1);
                row ++;
            }
            tasks_cnt++;
        }
        ptr = strtok(NULL, delim);
    }
    execute_commands(split_commands);
    return 0;
}