#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 100

void sendStr(int fd, char *str)
{
    int len;
    len = htonl(strlen(str));
    send(fd, &len, sizeof(len), 0); // Send len of the string
    send(fd, str, strlen(str), 0);  // Send the string
}

void recvStr(int fd, char *str)
{
    int len = 0;
    char content[MAXLINE];

    memset(&content, 0, MAXLINE);
    recv(fd, &len, sizeof(len), 0);
    recv(fd, content, ntohl(len), 0);
    strcpy(str, content);
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
    return ntohl(num);
}

// Array values validations
int valueInArr(int val, int arr[], int size)
{
    if (size == 0)
        return 1;
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == val)
            return 0;
    }
    return 1;
}

int getIndex(int *arr, int num, int select)
{
    int indx = 0;
    for (int i = 0; i < num; i++)
    {
        if (arr[i] == select)
        {
            indx = i;
            break;
        }
    }
    return indx;
}

int countAvailableSeats(int id[], int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (id[i] == 0)
        {
            count++;
        }
    }
    return count;
}
