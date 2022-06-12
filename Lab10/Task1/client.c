#include "client.h"
#define MESSAGE_LENGTH 256

int playing_as_O;
char *client_name;
char *cmd;
char *argument;
char cross_board[9];
char current_move = 'O';
pthread_mutex_t server_side_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t game_cond = PTHREAD_COND_INITIALIZER;
char buffer[MESSAGE_LENGTH + 1];
int server_socket;

// 1 - starting the game
// 2 - waiting for enemy to join
// 3 - waiting for enemy move
// 4 - callback after enemy move
// 5 - my move
// 6 - exiting
int game_state = 1;


int move(char *_cross_board, int position) {
    if (position < 0 || position > 9 || _cross_board[position] == 'X' || _cross_board[position] == 'O') return 0;
    _cross_board[position] = current_move;
    if (current_move == 'O'){
        current_move = 'X';
    }else{
        current_move = 'O';
    }
    return 1;
}

int is_filled(char cell) {
    return cell == 'X' || cell == 'O';
}
void end() {
    sprintf(buffer, "exit: :%s", client_name);
    send(server_socket, buffer, 256, 0);
    exit(0);
}
char check_winner(char *_cross_board) {
    char winner = ' ';

    for (int x = 0; x < 3; x++) {
        if (_cross_board[x] == _cross_board[x + 3] && _cross_board[x] == _cross_board[x + 6] &&
            is_filled(_cross_board[x])) {
            winner = _cross_board[x];
        }
    }
    for (int y = 0; y < 3; y++) {
        if (_cross_board[3 * y] == _cross_board[3 * y + 1] && _cross_board[3 * y] == _cross_board[3 * y + 2] &&
            is_filled(_cross_board[3 * y])) {
            winner = _cross_board[3 * y];
        }
    }
    if (_cross_board[0] == _cross_board[4] && _cross_board[0] == _cross_board[8] && is_filled(_cross_board[0])) {
        winner = _cross_board[0];
    }
    if (_cross_board[2] == _cross_board[4] && _cross_board[2] == _cross_board[6] && is_filled(_cross_board[2]))
        winner = _cross_board[2];
    return winner;
}
void has_game_ended() {
    int win = 0;
    char winner = check_winner(cross_board);

    if (winner != ' ') {
        if ((playing_as_O && winner == 'O') || (!playing_as_O && winner == 'X')) {
            printf("You won!\n");
        } else {
            printf("You lost!\n");
        }
        win = 1;
    }
    int tie = 1;
    for (int i = 0; i < 9; i++) {
        if (!is_filled(cross_board[i])) {
            tie = 0;
            break;
        }
    }
    if (tie) {
        printf("Tie! \n");
    }

    if (win == 1 || tie) game_state = 6;
}






void clear_cross_board() {
    for (int i = 0; i < 9; ++i) {
        cross_board[i] = ' ';
    }
}
void draw_the_board() {
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (!is_filled(cross_board[y * 3 + x])) {
                printf(" %d ", y * 3 + x + 1);
            } else if (cross_board[y * 3 + x] == 'O') {
                printf(" 0 ");
            } else {
                printf(" X ");
            }

        }
        printf("\n");
    }
}
void parse_command(char *msg) {
    cmd = strtok(msg, ":");
    argument = strtok(NULL, ":");
}



void game_thread_on_beginning(){
    printf("Starting! \n");
    if (strcmp(argument, "no_enemy") == 0) game_state = 2;
    else {
        clear_cross_board();
        playing_as_O = 0;
        if (argument[0] == 'O'){
            playing_as_O = 1;
        }
        if (playing_as_O){
            game_state = 5;
        }else{
            game_state = 3;
        }
    }
    printf("is client 0 : %d \n", playing_as_O);
    printf("Started! \n");
}
void waiting_for_enemy_to_join(){
    printf("Waiting for enemy to join! \n");
    pthread_mutex_lock(&server_side_mutex);
    while (game_state != 1 && game_state != 6) {
        pthread_cond_wait(&game_cond, &server_side_mutex);
    }

    pthread_mutex_unlock(&server_side_mutex);
    clear_cross_board();
    playing_as_O = 0;
    if (argument[0] == 'O'){
        playing_as_O = 1;
    }
    printf("is client 0 : %d \n", playing_as_O);
    if (playing_as_O){
        game_state = 5;
    }else{
        game_state = 3;
    }
    printf("Enemy joined! \n");
}

void wait_for_enemy_move(){
    printf("Enemy is making their move\n");

    pthread_mutex_lock(&server_side_mutex);
    while (game_state != 4 && game_state != 6) {
        pthread_cond_wait(&game_cond, &server_side_mutex);
    }
    pthread_mutex_unlock(&server_side_mutex);
    printf("Enemy has made their move! \n");
}

void on_enemy_move(){
    printf("Enemy move callback\n");
    int pos = atoi(argument);
    move(cross_board, pos);
    has_game_ended();
    if (game_state != 6) {
        game_state = 5;
    }
    printf("Enemy move callback done \n");
}

void my_move(){
    printf("Time for my move \n");
    draw_the_board();

    int index;
    do {
        if (playing_as_O){
            printf("Next move: O \n");
        }else{
            printf("Next move: X \n");
        }
        scanf("%d", &index);
        index--;
    } while (!move(cross_board, index));

    draw_the_board();

    char buffer[MESSAGE_LENGTH + 1];
    sprintf(buffer, "move:%d:%s", index, client_name);
    send(server_socket, buffer, MESSAGE_LENGTH, 0);

    has_game_ended();
    if (game_state != 6) {
        game_state = 3;
    }
}
void game_thread() {
    while (1) {
        if (game_state == 1) {
            game_thread_on_beginning();
        } else if (game_state == 2) {
            waiting_for_enemy_to_join();
        } else if (game_state == 3) {
            wait_for_enemy_move();
        } else if (game_state == 4) {
            on_enemy_move();
        } else if (game_state == 5) {
            my_move();
        } else if (game_state == 6) end();
    }
}

void unix_connection(char *path) {
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_un));
    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, path);
    connect(server_socket, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
}

void internet_connection(char *port) {
    struct addrinfo *addinfo_1;
    struct addrinfo addrinfo_2;
    memset(&addrinfo_2, 0, sizeof(struct addrinfo));
    addrinfo_2.ai_family = AF_UNSPEC;
    addrinfo_2.ai_socktype = SOCK_STREAM;
    getaddrinfo("localhost", port, &addrinfo_2, &addinfo_1);
    server_socket = socket(AF_UNSPEC, SOCK_STREAM, addinfo_1->ai_protocol);
    connect(server_socket, addinfo_1->ai_addr, addinfo_1->ai_addrlen);

    freeaddrinfo(addinfo_1);
}
int create_player() {
    pthread_t t;
    pthread_create(&t, NULL, (void *(*)(void *)) game_thread, NULL);
    return 1;
}

int on_add_command(int is_player_created){
    game_state = 1;
    if (!is_player_created)
        is_player_created = create_player();
    return 1;
}


void on_move_command() {
    game_state = 4;
}

void on_quit_command() {
    game_state = 6;
    exit(0);
}

void on_ping_command() {
    sprintf(buffer, "reply: :%s", client_name);
    send(server_socket, buffer, MESSAGE_LENGTH, 0);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Wrong number of arguments!\n");
        exit(1);
    }
    clear_cross_board();
    client_name = argv[1];

    signal(SIGINT, end);
    if (strcmp(argv[2], "unix") == 0) {
        char *path = argv[3];
        unix_connection(path);
    } else if (strcmp(argv[2], "inet") == 0) {
        char *port = argv[3];
        internet_connection(port);
    } else {
        exit(1);
    }

    sprintf(buffer, "add: :%s", client_name);
    send(server_socket, buffer, MESSAGE_LENGTH, 0);

    int is_player_created = 0;

    while (1) {
        recv(server_socket, buffer, MESSAGE_LENGTH, 0);
        parse_command(buffer);


        pthread_mutex_lock(&server_side_mutex);
        if (strcmp(cmd, "add") == 0) {
            is_player_created = on_add_command(is_player_created);
        } else if (strcmp(cmd, "move") == 0)
            on_move_command();
        else if (strcmp(cmd, "end") == 0)
            on_quit_command();
        else if (strcmp(cmd, "ping") == 0)
            on_ping_command();
        pthread_cond_signal(&game_cond);
        pthread_mutex_unlock(&server_side_mutex);
    }
}