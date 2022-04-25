#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"

int socketfd;

void setFd(int fd)
{
    socketfd = fd;
}

void sendListMovies()
{
    int num = 4;
    int id[] = {1, 2, 4, 9};
    char *name[] = {"The Kashmir Files", "RRR", "K.G.F: Chapter 2", "Baahubali: The Beginning"};

    sendInt(socketfd, num);

    for (int i = 0; i < num; i++)
    {
        sendInt(socketfd, id[i]);
        sendStr(socketfd, name[i]);
    }
}

void sendListCinemas()
{
    int num = 3;
    char id[] = {17, 24, 36};
    char *name[] = {"INOX Cinemas", "Cinemax Cinemas", "City Pulse Cinema"};

    sendInt(socketfd, num);

    for (int i = 0; i < num; i++)
    {
        sendInt(socketfd, id[i]);
        sendStr(socketfd, name[i]);
    }
}

void sendListTimes()
{
    int num = 4;
    char id[] = {1, 2, 3, 4};
    char *name[] = {"09:00", "12:15", "18:00", "23:30"};

    sendInt(socketfd, num);

    for (int i = 0; i < num; i++)
    {
        sendInt(socketfd, id[i]);
        sendStr(socketfd, name[i]);
    }
}

void sendListSeats()
{
    int row = 3;
    int col = 4;
    char id[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char status[] = {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0};

    sendInt(socketfd, row);
    sendInt(socketfd, col);

    for (int i = 0; i < row * col; i++)
    {
        sendInt(socketfd, id[i]);
        sendInt(socketfd, status[i]);
    }
}

void sendListPayments(int fare)
{
    int num = 2;
    int id[] = {1, 2};
    char *name[] = {"Thanh toan truc tiep tai rap", "Thanh toan online"};

    sendInt(socketfd, fare);
    sendInt(socketfd, num);
    for (int i = 0; i < num; i++)
    {
        sendInt(socketfd, id[i]);
        sendStr(socketfd, name[i]);
    }
}

void confirmOrder()
{
    // TODO: check the validity of the order
    sendInt(socketfd, SUCCESS);
}