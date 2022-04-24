#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define MAXLINE 100    /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8      /*maximum number of client connections */

int socketfd;
int child_process_running = 1;

enum
{
    SUCCESS,
    FAILURE,
    // LOGIN,
    SIGNUP,
    BOOKING,
    ORDERS,
    MOVIE,
    CINEMA,
    TIME,
    SEAT,
    PAY,
    CONFIRM
};

void initServer()
{
    struct sockaddr_in servaddr;
    // creation of the socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Server Socket is created.\n");

    // preparation of the socket address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if (bind(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("[-]Error in binding.\n");
        exit(1);
    }

    if (listen(socketfd, LISTENQ) == 0)
    {
        printf("[+]Listening....\n");
    }
    else
    {
        printf("[-]Error in binding.\n");
    }
}

void termination_handler(int signum)
{
    child_process_running = 0;
}

void sendStr(int fd, char *str)
{
    int len;
    len = htonl(strlen(str));
    send(fd, &len, sizeof(len), 0); // Send length of the string
    send(fd, str, strlen(str), 0);  // Send the string
}

void recvStr(int fd, char *str)
{
    int len = 0;
    char content[MAXLINE];

    memset(&content, 0, MAXLINE);
    recv(fd, &len, sizeof(len), 0);   // Receive length of the string
    recv(fd, content, ntohl(len), 0); // Receive the string
    strcpy(str, content);
    // printf("%s\n", str);
}

void sendInt(int fd, int i)
{
    int content = htonl(i);
    send(fd, &content, sizeof(content), 0);
}

int recvInt(int fd)
{
    int num;
    recv(fd, &num, sizeof(num), 0);
    // printf("%d\n", ntohl(num));
    return ntohl(num);
}

int main(int argc, char **argv)
{
    int connfd, n, state = 0;
    pid_t childpid;
    socklen_t clilen;
    char mess[MAXLINE], pkg[MAXLINE], uname[MAXLINE], passwd[MAXLINE];

    int num = 4, idx, len;
    char *name[] = {"Frozen", "Avenger", "IT", "Joker"};
    int id[] = {1, 2, 4, 9};

    struct sockaddr_in cliaddr;
    struct sigaction new_action, old_action;

    new_action.sa_handler = termination_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGINT, &new_action, &old_action); // Ctrl+C for generating the signal

    initServer();
    while (1)
    {
        clilen = sizeof(cliaddr);
        connfd = accept(socketfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0)
        {
            exit(1);
        }
        printf("\n[+]%s:%d - Connection accepted\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

        if ((childpid = fork()) == 0)
        {
            close(socketfd);
            while (child_process_running)
            {
                n = recv(connfd, &state, sizeof(state), 0);
                if (n < 0)
                {
                    perror("Read error");
                    exit(1);
                }

                if (n == 0)
                {
                    printf("[+]%s:%d - Disconnected\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                    break;
                }

                state = ntohl(state);
                // printf("%d\n", state);
                switch (state)
                {
                // case LOGIN:
                //     printf("[+]%s:%d - Request login from user ", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                //     recvStr(connfd, uname);
                //     recvStr(connfd, passwd);
                //     puts(uname);
                //     puts(passwd);
                //     sendInt(connfd, SUCCESS);
                //     printf("[+]%s:%d - Login successful\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

                //     break;
                case BOOKING:
                    printf("[+]%s:%d - Request BOOKING\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                    break;
                case MOVIE:
                    printf("[+]%s:%d - Request MOVIE\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                    sendInt(connfd, num);
                    for (int i = 0; i < num; i++)
                    {
                        sendInt(connfd, id[i]);
                        sendStr(connfd, name[i]);
                    }
                    printf("[+]%s:%d - Sent MOVIE\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

                    idx = recvInt(connfd);
                    recvStr(connfd, pkg);
                    printf("[+]%s:%d - Choose movie: %s - %u\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), pkg, idx);
                    break;
                default:
                    break;
                }
            }
        }
    }
    close(connfd);
}