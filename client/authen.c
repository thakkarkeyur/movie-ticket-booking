#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"

#define MAXLINE 4096

int logIn(int socketfd, char *uname)
{
    char passwd[MAXLINE];
    int state;

    printf("\nPlease enter your name: ");
    scanf("%s", uname);

    sendInt(socketfd, LOGIN);
    sendStr(socketfd, uname);

    state = recvInt(socketfd);
    return state;
}