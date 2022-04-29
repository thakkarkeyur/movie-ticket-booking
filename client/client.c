#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../lib/message.h"
#include "booking.h"

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/

int socketfd;
message mess;

void connectToServer()
{
    struct sockaddr_in servaddr;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
    }
    printf("Client Socket is created...\n");

    // Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(SERV_PORT); // convert to big-endian order

    // Connection of the client to the socket
    if (connect(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Problem in connecting to the server");
        exit(3);
    }
    printf("Connected to Server.\n");
}

int main(int argc, char **argv)
{
    int choice = 0;
    char uname[MAXLINE];

    connectToServer();

    username(socketfd, uname);
    booking(socketfd, uname);

    return 0;
}