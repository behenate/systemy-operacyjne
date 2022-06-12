#ifndef LAB10_COMMON_H
#define LAB10_COMMON_H


#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>

typedef struct{
    char* nickname;
    int id;
    int connected;
    int enemy_id;
} client;


#endif
