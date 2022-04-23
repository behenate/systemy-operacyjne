#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "header.h"

int server_queue;
int client_queue;
int my_idx;
void send_STOP();


void sent_LIST() {
    message mess = {
        .type = LIST,
        .sender_queue= client_queue
    };
    int mess_size = sizeof(message) - sizeof(long);
    msgsnd(server_queue, &mess, mess_size, 0);
    printf("Client has requested LIST\n");

    message response_message;
    msgrcv(client_queue, &response_message, mess_size, LIST, 0);
    printf("Client has received the list response\n");
    printf("All clients: %s \n", response_message.content);
}

int has_messages(){
    struct msqid_ds buf;
    msgctl(client_queue, IPC_STAT, &buf);
    return (buf.msg_qnum != 0);
}
void await_messages(){
    message mess;
    if (has_messages()) {
        int mess_size = sizeof(message) - sizeof(long);
        msgrcv(client_queue, &mess, mess_size, -6, IPC_NOWAIT);
        switch (mess.type) {
            case ALL:
                printf("Received a message!\ntype:ALL\n    from:%d\n    date:%s\n    text:%s\n", mess.sender_id, get_time(), mess.content);
                break;
            case ONE:
                printf("Received a message!\ntype:2ONE\n    from:%d\n    date:%s\n    text:%s\n", mess.sender_id, get_time(), mess.content);
                break;
            case STOP:
                send_STOP();
        }

    }
}

void send_ALL(char *text) {
    message mess = {
            .type = ALL,
            .sender_queue = client_queue,
            .receiver_id = -1,
            .sender_id=my_idx,
    };
    sprintf(mess.content, "%s", text);
    int mess_size = sizeof(message) - sizeof(long);
    msgsnd(server_queue, &mess, mess_size, 0);
    printf("Client %d has sent the send_ALL\n", my_idx);
}
void send_ONE(int receiver_id, char *text) {
    message mess = {
            .type = ONE,
            .sender_queue = client_queue,
            .receiver_id = receiver_id,
            .sender_id=my_idx,
    };
    sprintf(mess.content, "%s", text);
    int mess_size = sizeof(message) - sizeof(long);
    msgsnd(server_queue, &mess, mess_size, 0);
    printf("Client has sent the message : %s to %d\n", text, receiver_id);
}
void send_STOP(){
    message mess = {
            .type = STOP,
            .content = "Do widzenia",
            .sender_queue = client_queue,
            .receiver_id = -1,
            .sender_id=my_idx,
    };
    int mess_size = sizeof(message) - sizeof(long);
    msgsnd(server_queue, &mess, mess_size, 0);
    printf("Client has sent the STOP notification\n");
    exit(0);
}
void clear_client_queue(){
    msgctl(client_queue, IPC_RMID, NULL);
}

int send_init() {
    message mess = {
            .type = INIT,
            .content = "Dzień dobry",
            .sender_queue = client_queue,
            .receiver_id = -1,
            .sender_id=-1,
    };
    int mess_size = sizeof(message) - sizeof(long);
    msgsnd(server_queue, &mess, mess_size, 0);
    printf("Client has sent the init\n");
    message response_message;


    msgrcv(client_queue, &response_message, mess_size, INIT, 0);
    printf("Client has received the init response\n");
    my_idx = response_message.receiver_id;
    printf("Clients idx: %d \n", my_idx);
}

int main(int argc, char **argv) {
    atexit(clear_client_queue);
    signal(SIGINT, send_STOP);
    int server_key;
    server_key = ftok(getenv("HOME"), PROJECT_ID);
    server_queue = msgget(server_key, 0);

    srand(time(NULL));
    int id = getpid();
    int client_key;
    client_key = ftok(getenv("HOME"), id);
    client_queue = msgget(client_key, IPC_CREAT | IPC_EXCL | 0666);
    if(client_queue == -1){
        printf("Retrying \n");
        sleep(2);
        client_key = ftok(getenv("HOME"), id);
        client_queue = msgget(client_key, IPC_CREAT | IPC_EXCL | 0666);
        if (client_queue == -1){
            printf("Something went wrong, please start the client again!");
            exit(1);
        }

    }
    printf("server queue: %d, client queue: %d", server_queue, client_key);
    printf("Client sending INIT to: %d\n", server_key);
    send_init();
    int child = fork();
    while (1){

        if(child== 0){
            char  command[1000];
            char * command2 = calloc(1000, sizeof (char ));
            fgets(command, MAX_MESSAGE, stdin);
            printf("%s", command);
            if(strstr(command, "ALL") || strstr(command, "all")){
                char * message = calloc(MAX_MESSAGE, sizeof (char ));
                printf("%s", command);
                sscanf(command, "%s %[^\n]",command2, message);
                printf("SENDING TO ALL: %s \n", message);
                send_ALL(message);
            }else if(strstr(command, "ONE") || strstr(command, "one")){
                char * message = calloc(MAX_MESSAGE, sizeof (char ));
                int send_to = 0;

                sscanf(command, "%s %d %[^\n]",command2, &send_to, message);
                printf("SENDING ONE: %d %s \n", send_to, message);
                send_ONE(send_to, message);
            }else if(strstr(command, "LIST") || strstr(command, "list")){
                sent_LIST();
            }else if(strstr(command, "STOP") || strstr(command, "list")){
                send_STOP();
            }

        }else{
            await_messages();
        }
        sleep(1);
    }
}