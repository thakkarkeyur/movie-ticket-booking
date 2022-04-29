#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"

int socketfd;
char status[12];

void setFd(int fd)
{
    socketfd = fd;
}

void sendListMovies()
{
    int num = 6;
    int id[] = {1, 2, 4, 9};

    char *movies[6];
    char *movie;
    size_t len = 0;
    ssize_t read;

    FILE *fptr;
    fptr = fopen("../movies.txt","r");
    if(fptr == NULL){
        printf("Error in file opening for print menu. \n");
        return;
    }
    int i=0;
    while ((read = getline(&movie, &len, fptr)) != -1) {
        movies[i] = malloc(read);

        memcpy(movies[i], movie, read);
        i++;
      
    }

    sendInt(socketfd, num);

    for (int i = 0; i < 6; i++)
    {
        sendInt(socketfd, i+1);
        sendStr(socketfd, movies[i]);
    }
}

void sendListCinemas()
{
    int num = 5;
    char *cinema;
    int SIZE = 256;
    int i=0;
    char cin[SIZE];
    char *cinemas[5];

      FILE *fptr;
    fptr = fopen("../cinemas.txt","r");

    if(fptr == NULL){
        printf("Error in file opening for print menu. \n");
        return;
     }

    while(1)
    {
        fgets(cin, SIZE, fptr);
        cinemas[i] = malloc(sizeof cinema);
        strcpy(cinemas[i], cin);
        i++;
        if(feof(fptr))
            break;
    }


    sendInt(socketfd, num);

    for (int i = 0; i < num; i++)
    {
        sendInt(socketfd, i + 1);
        sendStr(socketfd, cinemas[i]);
    }
     fclose(fptr);
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
    char id[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    FILE *fptr;
    fptr = fopen("../seats.txt","r+");
    if(fptr == NULL){
        printf("Error in file opening for book a seat. \n");
        return ;
    }

    fseek(fptr,0,SEEK_SET);
    char ch;
    int i=0;
    int temp[12];
    int dtemp[12] = {0};
    while( (ch=fgetc(fptr)) != EOF){
        if(ch=='\n'){
            continue;
        }
        temp[i]= ch - '0';
        i++;
    }
    fclose(fptr);
    printf("\n");
    for(int i=0;i<12;i++){
        status[i] = temp[i];
    }

    sendInt(socketfd, row);
    sendInt(socketfd, col);

    for (int i = 0; i < row * col; i++)
    {
        sendInt(socketfd, id[i]);
        sendInt(socketfd, status[i]);
    }
}

void confirmOrder(int seatNum)
{
    printf("SeatNum is:   %d \n",seatNum);
    status[seatNum] = 1;

    FILE *fptr;
    fptr = fopen("../seats.txt","r+");
    if(fptr == NULL){
        printf("Error in file opening for book a seat. \n");
        return ;
    }
    fclose(fptr);

    fptr = fopen("../seats.txt","r+");
    if(fptr == NULL){
        printf("Error in file opening for book a seat. \n");
        return ;
    }

    fseek(fptr,0,SEEK_SET);
    char ch;
    int i=0;
    while( (ch=fgetc(fptr)) != EOF){
        if(ch=='\n'){
            continue;
        }
        status[i]= ch - '0';

    }
    fclose(fptr);

    printf("\n");
    sendInt(socketfd, SUCCESS);
}