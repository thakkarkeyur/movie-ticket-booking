#include <stdio.h>
#include <string.h>
#include "../lib/message.h"
#include "../lib/function.h"

#define MAXLINE 4096

void username(int socketfd)
{
    char uname[MAXLINE];

    recvStr(socketfd, uname);
    sendInt(socketfd, SUCCESS);
    puts(uname);
}