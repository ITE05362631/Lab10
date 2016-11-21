#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "doprocessing.h"
#include <pthread.h>

void *connection_handler(void *);
int main(int atgc, char *argv[])
{
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    int pid;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Connot create socket");
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");
    listen(socket_desc, 3);
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        if (new_socket < 0)
        {
            perror("accept failed");
            return 1;
        }
        else{
            pthread_t new_thread;
            int *new_sock = malloc(1);
            *new_sock = new_socket;
            if (pthread_create(&new_thread, NULL, connection_handler, (void *)new_sock) < 0)
            {
                perror("could not create thread");
                return 1;
            }
            // pid = fork();
            // if(pid == 0){
            //     close(socket_desc);
            //     doprocessing(new_socket);
            //     exit(0);
            // }
            // else{
            //     close(new_socket);
            // }
        }
    }
    return 0;
}
void *connection_handler(void *socket_desc)
{
//Get the socket descriptor
int sock = *(int*)socket_desc;
doprocessing(sock);
// char *message;
// //Send some messages to the client
// message = "Greetings! I am your connection handler\n";
// write(sock , message , strlen(message));
// message = "Its my duty to communicate with you";
// write(sock , message , strlen(message));
// //Free the socket pointer
// free(socket_desc);
// return 0;
}