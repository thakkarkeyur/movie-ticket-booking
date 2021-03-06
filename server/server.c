#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "booking.h"
#include "../lib/message.h"
#include "../lib/function.h"

#define MAXLINE 4096   // Max text line length
#define SERV_PORT 3000 // Port number
#define LISTENQ 8      // Maximum number of client connections

int socketfd;
order ticket;
int child_process_running = 1;

void initServer()
{
    struct sockaddr_in servaddr;
    // creation of the socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET - IPv4   // SOCK_STREAM - TCP
    if (socketfd < 0)
    {
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Server Socket is created...\n");

    // preparation of the socket address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("Error in binding.\n");
        exit(1);
    }

    if (listen(socketfd, LISTENQ) == 0)
    {
        printf("Listening....\n");
    }
    else
    {
        printf("Error in binding.\n");
    }
}

int main()
{
    int connfd, n, state;
    pid_t childpid;   // Storing the child process ID
    socklen_t clilen; // Storing socket length of client
    struct sockaddr_in cliaddr;

    initServer();
    while (1)
    {
        clilen = sizeof(cliaddr);
        connfd = accept(socketfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0)
        {
            exit(1);
        }
        printf("\nConnection accepted...\n");

        if ((childpid = fork()) == 0)
        {
            close(socketfd);
            while (1)
            {
                n = recv(connfd, &state, sizeof(state), 0);
                if (n < 0)
                {
                    perror("Read error");
                    exit(1);
                }

                if (n == 0)
                {
                    printf("Disconnected...\n");
                    exit(0);
                }

                state = ntohl(state);
                switch (state)
                {
                case USERNAME:
                    username(connfd);
                    break;

                case BOOKING:
                    printf("\nRequest BOOKING...\n");
                    setFd(connfd);
                    break;

                case MOVIE:
                    printf("\nRequest MOVIE...\n");
                    sendListMovies(connfd);
                    ticket.movie_id = recvInt(connfd);
                    recvStr(connfd, ticket.movie);
                    printf("Sent MOVIE...\n");
                    printf("Choose movie: %s\n", ticket.movie);
                    break;

                case CINEMA:
                    printf("\nRequest CINEMA...\n");
                    sendListCinemas(connfd);
                    ticket.cinema_id = recvInt(connfd);
                    recvStr(connfd, ticket.cinema);
                    printf("Sent CINEMA...\n");
                    printf("Choose cinema: %s\n", ticket.cinema);
                    break;

                case TIME:
                    printf("\nRequest TIME...\n");
                    sendListTimes(connfd);
                    ticket.time_id = recvInt(connfd);
                    recvStr(connfd, ticket.time);
                    printf("Sent TIME...\n");
                    printf("Choose time: %s\n", ticket.time);
                    break;

                case SEAT:
                    printf("\nRequest SEAT...\n");
                    sendListSeats(connfd);
                    printf("Sent SEAT...\n");
                    FILE *fptr;
                    fptr = fopen("../seats.txt", "r+");
                    if (fptr == NULL)
                    {
                        printf("Error in file opening for book a seat. \n");
                        return 0;
                    }

                    ticket.seat_num = recvInt(connfd);
                    for (int i = 0; i < ticket.seat_num; i++)
                    {
                        ticket.seat_id[i] = recvInt(connfd);
                        fseek(fptr, ticket.seat_id[i], SEEK_SET);
                        fputs("1", fptr);
                    }
                    printf("Choosen seat(s): ");
                    for (int i = 0; i < ticket.seat_num; i++)
                    {
                        printf(" %d", ticket.seat_id[i]);
                    }
                    printf("\n");
                    break;

                case CONFIRM:
                    printf("\nOrder Confirmed...\n");
                    confirmOrder(ticket.seat_id[0]);
                    break;

                default:
                    break;
                }
            }
        }
    }
    close(connfd);
    return 0;
}