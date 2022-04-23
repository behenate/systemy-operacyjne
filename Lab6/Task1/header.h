#include <time.h>

#define PROJECT_ID  2341
#define CLIENT_PROJ_ID getpid()


#define STOP 1
#define LIST 2
#define ALL 3
#define ONE 4
#define INIT 5


#define MAX_CLIENTS 5
#define MAX_MESSAGE 256

typedef struct {
    long type;
    char content[MAX_MESSAGE];
    int sender_queue;
    int sender_id;
    int receiver_id;
    char * date;
} message;

typedef struct {
    int id;
    int client_queue;
    int connected;
} client;

char* get_time(){
    char * time_s = calloc(100, sizeof (char ));
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(time_s," %d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return time_s;
}