#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "header.h"
#include <mqueue.h>
int server_queue;
int client_queue;
char client_name[100];
int my_idx;
void send_STOP();



int has_messages(){
    struct mq_attr buf;
    mq_getattr(client_queue, &buf);
    return (buf.mq_curmsgs != 0);
}
void await_messages(){
    message mess;
    if (has_messages()) {
        unsigned int prio;
        mq_receive(client_queue, (char *) &mess, sizeof (message), &prio);
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
    mq_send(server_queue, (const char *) &mess, sizeof (message), ALL);
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
    mq_send(server_queue, (const char *) &mess, sizeof (message), ONE);
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
    mq_send(server_queue, (const char *) &mess, sizeof (message), STOP);
    printf("Client has sent the STOP notification\n");
    exit(0);
}
void clear_client_queue(){
    mq_close(client_queue);
    mq_close(server_queue);
    mq_unlink(client_name);
}

void sent_LIST() {
    message mess = {
            .type = LIST,
            .sender_queue= client_queue,
            .sender_id = my_idx
    };
//    msgsnd(server_queue, &mess, mess_size, 0);
    mq_send(server_queue, (char*) &mess, sizeof (message), LIST);
    printf("Client has requested LIST\n");

    message response_message;
//    msgrcv(client_queue, &response_message, mess_size, LIST, 0);
    unsigned int prio;
    printf("%d \n", client_queue);
    mq_receive(client_queue, (char*) &response_message, sizeof (message), &prio);
    printf("Client has received the list response\n");
    printf("All clients: %s \n", response_message.content);
}


int send_init() {
    message mess = {
            .type = INIT,
            .sender_queue = client_queue,
            .content = "h11111111111111"
    };
    strcpy(mess.content, client_name);
    mq_send(server_queue, (char*) &mess, sizeof (message), INIT);
    printf("Client has sent the init\n");

    message response_message;

    usleep(100000);
    unsigned int prio;

    long res = mq_receive(client_queue, (char*)&response_message, sizeof (message), &prio);

    printf("Client has received the init response %s\n", response_message.content);
    my_idx = response_message.receiver_id;

    printf("Clients idx: %d \n", my_idx);
}

int main(int argc, char **argv) {
    atexit(clear_client_queue);
    signal(SIGINT, send_STOP);

    server_queue = mq_open(S_QUEUE_NAME, O_WRONLY);


    struct mq_attr attributes;
    attributes.mq_maxmsg = MAX_MESSAGES;
    attributes.mq_msgsize = sizeof (message);
    attributes.mq_flags = 0;
    attributes.mq_curmsgs = 0;
    sprintf(client_name, "/cq_%d", getpid());
    client_queue = mq_open(client_name, O_RDONLY | O_CREAT | O_EXCL, 0777, &attributes);

    printf("server queue: %d, client queue: %d", server_queue, client_queue);
    printf("Client sending INIT to: %d\n", server_queue);
    send_init();
    sent_LIST();
    int child = fork();
    while (1){

        if(child== 0){
            char  command[1000];
            char * command2 = calloc(1000, sizeof (char ));
            fgets(command, MAX_MESSAGE, stdin);
            printf("%s", command);
            if(strstr(command, "ALL") || strstr(command, "all")){
                char * message = calloc(MAX_MESSAGE, sizeof (char ));
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