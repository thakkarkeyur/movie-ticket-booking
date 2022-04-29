#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"

#define MAXLINE 4096

int username(int socketfd, char *uname)
{
    int state;

    printf("\nPlease enter your name: ");
    scanf("%s", uname);

    sendInt(socketfd, USERNAME);
    sendStr(socketfd, uname);

    state = recvInt(socketfd);
    return state;
}