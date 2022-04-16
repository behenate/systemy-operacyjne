#include <stdio.h>
#include <malloc.h>
#include <string.h>

void send_mail(char* mail, char* title, char* contents){
    char * command = malloc(10000);
    sprintf(command, "echo %s | mail -s %s %s", contents, title, mail);
    printf(command);
    FILE *popened = popen(command, "w");
    pclose(popened);
}
void read_mail(char *sorting){
    if(strcmp("nadawca", sorting) == 0){
        FILE *popened = popen("echo | mail -f | tail +2 | head -n -1 | sort -k 2", "w");
        pclose(popened);
    }else{
        FILE *popened = popen("mail", "w");
        pclose(popened);
    }
}
int main(int argc, char** argv){
    int n;
    char* sender_date = calloc(100, sizeof (char));
    char* email = calloc(100, sizeof (char));
    char* title = calloc(1000, sizeof (char));
    char* contents = calloc(1000, sizeof (char));


    if(argc == 2){
        sender_date = argv[1];
        read_mail(argv[1]);
    } else if (argc==4){
        email = argv[1];
        title = argv[2];
        contents = argv[3];
        send_mail(email, title, contents);
    }
}