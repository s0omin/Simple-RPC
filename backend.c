#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a1_lib.h"

int main(int argc, char *argv[]) {

    int sockfd, clientfd;
    char msg[1024];
    char result[1024];

    if (argc < 3) {
        fprintf(stderr, "ERROR: host and port are required");
        fflush(stdout);
        return -1;
    }

    else if (create_server(argv[1], atoi(argv[2]), &sockfd) < 0){
        fprintf(stderr, "ERROR: Can't create server");
        fflush(stdout);
        return -1;
    }

    printf("Server listening on %s:%s\n", argv[1], argv[2]);
    fflush(stdout);

    int done = 0;
    int id, rval;
    int pid[10] = { NULL };

    for (int i=0; i < 10; i++) {
        if (pid[i] == NULL) {
            pid[i]= getpid();
            break;
        }
    }

    int *ptr = pid;


    while(!done) {

        if (accept_connection(sockfd, &clientfd) < 0) {
            fprintf(stderr,"Can't accept connection\n");
            fflush(stdout);
            return -1;
        }

        if ((clientfd - 3) > 5 ) {
            fprintf(stderr,"Too many connections\n");
            fflush(stdout);
            close(clientfd);
            continue;
        }

        id = fork();
        if (id == 0) {
            ptr++;
            *ptr = getpid();

            while (strcmp(msg, "exit\n")){
                memset(msg, 0, sizeof(msg));
                ssize_t byte = recv_message(clientfd, msg, 1024);

                if (byte <= 0){
                    break;
                }
                
                printf("Client: %s\n", msg);
                fflush(stdout);

                msg[strlen(msg)-1] = '\0';

                char *data[3];
                char *p = strtok (msg, " ");
                    int i = 0;

                while (p) {
                    data[i++] = p;
                    p = strtok(NULL, " ");
                }

                if (strcmp(data[0], "add") == 0){
                    int ans = atoi(data[1]) + atoi(data[2]);
                    snprintf(result, sizeof(result), "%d", ans);
                }

                else if (strcmp(data[0], "multiply") == 0){
                    int ans = atoi(data[1]) * atoi(data[2]);
                    snprintf(result, sizeof(result), "%d", ans);
                }

                else if (strcmp(data[0], "divide") == 0) {
                    if (atof(data[2]) == 0){
                        strcpy(result, "Division by zero");
                    }
                    else {
                        float ans = atof(data[1]) / atof(data[2]);
                        snprintf(result, sizeof(result), "%f", ans);
                    }
                }

                else if (strcmp(data[0], "factorial") == 0) {
                    uint64_t ans = atoi(data[1]);
                    for (int temp = ans-1; temp > 1; temp--){
                        ans *= temp;
                    }
                    snprintf(result, sizeof(result), "%ld", ans);
                }
                else if (strcmp(data[0], "sleep") == 0) {
                    int time = atoi(data[1]);
                    sleep(time);
                }
                else if (strcmp(data[0], "exit") == 0) {
                    strcpy(result, "Bye!");
                }
                else if (strcmp(data[0], "quit") == 0 || strcmp(data[0], "shutdown") == 0) {
                    strcpy(result, "Bye!");
                    done = 1;
                    break;
                }
                else {
                    snprintf(result, sizeof(result), "Command \"%s\" not found", data[0]);
                }
                send_message(clientfd, result, 1024);
                *result = '\0';
                fflush(stdout);

                if (strcmp(data[0], "exit") == 0) {
                    close(clientfd);
                }
            }
            if (done) {
                printf("Shutting Down\n");
                close(clientfd);

            }
        }
        else if (id < 0) {
            fprintf(stderr, "ERROR: Unable to fork");
            fflush(stdout);
            return -1;
        }
    } 
    printf ("Backend terminated.\n");
    fflush(stdout);
    return 0;
}


