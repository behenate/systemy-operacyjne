#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


#include "header.h"

client clients[MAX_CLIENTS];
int queue;
FILE * log_file;

void save_to_file(message * mess){
    fprintf(log_file, "Message:\n   type:%ld\n   sender: %d,\n   receiver: %d,\n   time:%s,\n   text:%s\n",mess->type, mess->sender_id,mess->receiver_id, get_time(), mess->content);
}
void handle_LIST(message *mess);

void handle_ALL(message *mess);

void handle_exit() {
    printf("Closing children \n");
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].connected){
            message resp = {
                    .type = STOP,
            };
            msgsnd(clients[i].client_queue, &resp, sizeof(message) - sizeof(long), 0);
        }
    }
    sleep(1);

    fclose(log_file);
    exit(0);
}
void close_server_queue(){
    msgctl(queue, IPC_RMID, NULL);
}

int find_free_idx() {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].connected == 0) {
            return i;
        }
    }
    return -1;
}

void handle_INIT(message *mess) {
    printf("Received init from: %d \n", mess->sender_queue);
    int assigned_idx = find_free_idx();
    client new = {
            .id = assigned_idx,
            .client_queue = mess->sender_queue,
            .connected = 1
    };
    if(assigned_idx == -1){
        printf("No place for client!\n");
        return;
    }
    clients[assigned_idx] = new;
    message resp = {
            .sender_queue = queue,
            .receiver_id = assigned_idx,
            .type = INIT,
    };
//    Send response to client with the assigned idx
    msgsnd(mess->sender_queue, &resp, sizeof(message) - sizeof(long), 0);
}
void handle_LIST(message *mess) {
    printf("Printing all active clients: \n");
    message resp = {
            .type = LIST,
            .sender_queue = queue,
            .receiver_id = mess->sender_id
    };
    char tagstr[MAX_MESSAGE];
    int pos = 0;
    int i;

    for (i=0; i < MAX_CLIENTS; i++) {
        if (clients[i].connected){
            printf("Client id: %d \n", clients[i].id);
            pos += sprintf(&tagstr[pos], "%d, ", i);
        }
    }
    strcpy(resp.content, tagstr);
    printf ("All clients: %s\n", tagstr);
    msgsnd(mess->sender_queue, &resp, sizeof(message) - sizeof(long), 0);
    printf("Server has sent the clients\n");
}

void handle_ALL(message *mess) {
    printf("Sending message to all active clients: %s \n", mess->content);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].connected && clients[i].id != mess->sender_id){
            printf("  Sending to: %d \n", i);
            msgsnd(clients[i].client_queue, mess, sizeof(message) - sizeof(long), 0);
        }
    }
}

void handle_ONE(message *mess) {
    printf("Sending to %d message: %s \n",mess->receiver_id, mess->content);
    int i = mess->receiver_id;
    if (clients[i].connected && clients[i].id != mess->sender_id){
        msgsnd(clients[i].client_queue, mess, sizeof(message) - sizeof(long), 0);
    }else{
        printf("Invalid receiver id!\n");
    }

}
void handle_STOP(message * mess){
    int i = mess ->sender_id;
    clients[i].connected = 0;
    printf("Client %d successfully disconnected \n", i);
}

void process_message(message *mess) {
    switch (mess->type) {
        case INIT:
            handle_INIT(mess);
            break;
        case LIST:
            handle_LIST(mess);
            break;
        case ALL:
            handle_ALL(mess);
            break;
        case ONE:
            handle_ONE(mess);
            break;
        case STOP:
            handle_STOP(mess);
            break;
        default:
            printf("Error: %ld \n", mess->type);
            break;
    }
    save_to_file(mess);
}




int main(int argc, char **argv) {
    log_file = fopen("log_file.txt", "w+");
    int key;
    key = ftok(getenv("HOME"), PROJECT_ID);

    queue = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    //Close prev queue
    atexit(close_server_queue);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        client new = {.connected = 0};
        clients[i] = new;
    }
    signal(SIGINT, handle_exit);
    message mess;
    printf("Server starting to wait, key: %d \n", key);
    while (1) {
        int mess_size = sizeof(message) - sizeof(long);
        msgrcv(queue, &mess, mess_size, -6, 0);
        process_message(&mess);
        usleep(50000);
    }
}