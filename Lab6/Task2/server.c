#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <mqueue.h>
#include <errno.h>
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
            int client_queue = mq_open(clients[i].client_queue_name, O_WRONLY);
            mq_send(client_queue, (const char *) &resp, sizeof(message), STOP);
        }
    }
    sleep(1);

    fclose(log_file);
    exit(0);
}
void close_server_queue(){
    printf("EXIT \n");
    mq_close(queue);
    mq_unlink(S_QUEUE_NAME);
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
            .connected = 1
    };
    sprintf(new.client_queue_name, "%s", mess->content);
    if(assigned_idx == -1){
        printf("No place for client!\n");
        return;
    }

    int client_queue = mq_open(mess->content, O_WRONLY);
    new.client_queue = client_queue;
    clients[assigned_idx] = new;
    printf("Init reply to: %d \n", client_queue);

    message resp = {
            .sender_queue = queue,
            .receiver_id = assigned_idx,
            .type = INIT,
            .content = "Hello"
    };
//    Send response to client with the assigned idx
    mq_send(client_queue, (char * ) &resp, sizeof(message), INIT);
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
    int client_queue = mq_open(clients[mess->sender_id].client_queue_name, O_WRONLY);
    mq_send(client_queue, (char * ) &resp, sizeof(message), LIST);
    printf("Server has sent the clients\n");
}

void handle_ALL(message *mess) {
    printf("Sending message to all active clients: %s \n", mess->content);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].connected && clients[i].id != mess->sender_id){
            printf("  Sending to: %d %s \n", i,clients[i].client_queue_name);
            int client_queue = mq_open(clients[i].client_queue_name, O_WRONLY);
            mq_send(client_queue, (char*) mess, sizeof(message), ALL);
        }
    }
}

void handle_ONE(message *mess) {
    printf("Sending to %d message: %s \n",mess->receiver_id, mess->content);
    int i = mess->receiver_id;
    int client_queue = mq_open(clients[i].client_queue_name, O_WRONLY);
    if (clients[i].connected && clients[i].id != mess->sender_id){
        mq_send(client_queue, (char *) mess, sizeof(message), ONE);
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
    printf("%s \n",mess->content);
    save_to_file(mess);
}




int main(int argc, char **argv) {
    mq_unlink(S_QUEUE_NAME);
    log_file = fopen("log_file.txt", "w+");
    int server_q;
    struct mq_attr attr;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(message);
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;

    // open a queue with the attribute structure
    queue = mq_open(S_QUEUE_NAME,  O_RDONLY | O_CREAT | O_EXCL, 0777, &attr);
    printf("server queue: %d", server_q);
    if (queue == -1){
        printf("Error while creating queue! %s\n", strerror(errno));
        exit(0);
    }
    atexit(close_server_queue);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        client new = {.connected = 0};
        clients[i] = new;
    }
    signal(SIGINT, handle_exit);
    message mess;

    unsigned int prio;
    while (1) {
        mq_receive(queue, (char *) &mess, sizeof(message), &prio);
        process_message(&mess);
        usleep(500000);
    }
}