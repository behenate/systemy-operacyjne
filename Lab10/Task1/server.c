#include "client.h"

#define CLIENTS_NUM 16
#define MESSAGE_LENGTH 256
pthread_mutex_t server_side_mutex = PTHREAD_MUTEX_INITIALIZER;

client *clients[CLIENTS_NUM] = {NULL};
int connected_clients = 0;

int find_enemy_idx(int i) {
    if (i % 2 == 0) {
        return i + 1;
    }
    return i - 1;
}

client *register_client(char *name, int fd) {
    client *new_client = calloc(1, sizeof(client));
    new_client->nickname = calloc(MESSAGE_LENGTH, sizeof(char));
    strcpy(new_client->nickname, name);
    new_client->id = fd;
    new_client->connected = 1;
    return new_client;
}
int get_enemy_client_idx(){
    int client_idx = -1;
    for (int i = 0; i < CLIENTS_NUM; i++) {
        if (i % 2 == 0) {
            if (clients[i] != NULL && clients[i + 1] == NULL) {
                client_idx = i + 1;
                break;
            }
        }
    }
    return client_idx;
}
int get_alone_client_idx(){
    int client_idx = -1;
    for (int i = 0; i < CLIENTS_NUM; i++) {
        if (clients[i] == NULL) {
            client_idx = i;
            break;
        }
    }
    return client_idx;
}
int get_client_idx() {
    int client_idx = get_enemy_client_idx();
    if (client_idx == -1) {
        client_idx = get_alone_client_idx();
    }
    return client_idx;
}

int add_client(char *name, int fd) {
    int client_idx = get_client_idx();
    if (client_idx == -1)
        return client_idx;

    clients[client_idx] = register_client(name, fd);
    connected_clients++;
    return client_idx;
}


int get_idx_by_name(char *name) {
    for (int i = 0; i < CLIENTS_NUM; i++) {
        if (clients[i] == NULL)
            continue;
        if (strcmp(clients[i]->nickname, name) == 0)
            return i;
    }
    return -1;
}

void free_client(int index) {
    free(clients[index]->nickname);
    free(clients[index]);
    clients[index] = NULL;
    connected_clients--;

    if (clients[find_enemy_idx(index)] != NULL) {
        free_client(find_enemy_idx(index));
    }
}

void remove_client(char *name) {
    int client_index = -1;
    for (int i = 0; i < CLIENTS_NUM; i++) {
        if (clients[i] != NULL && strcmp(clients[i]->nickname, name) == 0){
            client_index = i;
        }
    }
    free_client(client_index);
    printf("Deleted client: %s\n", name);
}


void remove_disconnected_clients() {
    for (int i = 0; i < CLIENTS_NUM; i++) {
        if (clients[i] && !clients[i]->connected) {
            remove_client(clients[i]->nickname);
        }
    }
}

void *ping_thread() {
    while (1) {
        printf("Pinging the clients\n");
        pthread_mutex_lock(&server_side_mutex);
        remove_disconnected_clients();
        for (int i = 0; i < CLIENTS_NUM; i++) {
            if (clients[i]) {
                send(clients[i]->id, "ping: ", MESSAGE_LENGTH, 0);
                clients[i]->connected = 0;
            }
        }
        pthread_mutex_unlock(&server_side_mutex);
        sleep(2);
    }
}

int create_socket_l(char *socket_path) {
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_un));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, socket_path);
    unlink(socket_path);
    bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
    listen(sock_fd, CLIENTS_NUM);
    return sock_fd;
}

int create_socket_n(char *port) {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sock_addr;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(atoi(port));
    bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr));
    listen(sock_fd, CLIENTS_NUM);
    return sock_fd;
}


int get_client_fd(int local_socket, int network_socket) {
    struct pollfd *pools = calloc(2 + connected_clients, sizeof(struct pollfd));
    pools[0].fd = local_socket;
    pools[0].events = POLLIN;
    pools[1].fd = network_socket;
    pools[1].events = POLLIN;
    pthread_mutex_lock(&server_side_mutex);
    for (int i = 2; i < connected_clients + 2; i++) {
        pools[i].fd = clients[i-2]->id;
        pools[i].events = POLLIN;
    }
    pthread_mutex_unlock(&server_side_mutex);
    poll(pools, connected_clients + 2, -1);
    int client_fd;
    for (int i = 0; i < connected_clients + 2; i++) {
        if (pools[i].revents & POLLIN) {
            client_fd = pools[i].fd;
            break;
        }
    }
    if (client_fd == local_socket || client_fd == network_socket) {
        client_fd = accept(client_fd, NULL, NULL);
    }
    free(pools);
    return client_fd;
}
void add_clients_cmd(char * client_name, int client_fd){
    printf("A client is being added. \n");
    int index = add_client(client_name, client_fd);
    if (index % 2 == 0) {
        send(client_fd, "add:no_enemy", MESSAGE_LENGTH, 0);
    } else {
        int assigned_idx = rand() % 100;
        int first_player_idx;
        int second_player_idx;
        if (assigned_idx % 2 == 0) {
            first_player_idx = index;
            second_player_idx = find_enemy_idx(index);
        } else {
            second_player_idx = index;
            first_player_idx = find_enemy_idx(index);
        }

        send(clients[first_player_idx]->id, "add:O",
             MESSAGE_LENGTH, 0);
        send(clients[second_player_idx]->id, "add:X",
             MESSAGE_LENGTH, 0);
    }
}
void move_clients_cmd(char *argument, char* client_name, char*buffer){
    int move = atoi(argument);
    int player = get_idx_by_name(client_name);

    sprintf(buffer, "move:%d", move);
    send(clients[find_enemy_idx(player)]->id, buffer, MESSAGE_LENGTH,
         0);
}
void ping_cmd(char* client_name){
    printf("Got a ping reply! \n");
    int player = get_idx_by_name(client_name);
    if (player != -1) clients[player]->connected = 1;
}

void client_communication_routine(int local_socket, int network_socket) {
    int client_fd = get_client_fd(local_socket, network_socket);
    char buffer[MESSAGE_LENGTH + 1];
    recv(client_fd, buffer, MESSAGE_LENGTH, 0);

    char *cmd = strtok(buffer, ":");
    char *argument = strtok(NULL, ":");
    char *client_name = strtok(NULL, ":");

    pthread_mutex_lock(&server_side_mutex);
    if (strcmp(cmd, "add") == 0) {
        add_clients_cmd(client_name, client_fd);
    }
    if (strcmp(cmd, "move") == 0) {
        move_clients_cmd(argument, client_name, buffer);
    }
    if (strcmp(cmd, "exit") == 0) {
        remove_client(client_name);
    }

    if (strcmp(cmd, "reply") == 0) {
        ping_cmd(client_name);
    }
    pthread_mutex_unlock(&server_side_mutex);
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Input three arguments!\n");
        exit(1);
    }
    srand(getpid());

    char *port = argv[1];
    char *socket_path = argv[2];

    int local_socket = create_socket_l(socket_path);
    int network_socket = create_socket_n(port);
    printf("Socket created\n");
    pthread_t t;
    pthread_create(&t, NULL, &ping_thread, NULL);
    while (1) {
        client_communication_routine(local_socket, network_socket);
    }
}